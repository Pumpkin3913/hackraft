#include "server.h"

#include "player.h"
#include "screen.h"
#include "aspect.h"
#include "tile.h"
#include "luawrapper.h"
#include "error.h"

#include <thread>

#ifdef __linux__
#include <unistd.h> // close()
#include <sys/socket.h> // socket(), bind(), listen()
#include <netinet/in.h> // IPv4, IPv6
#include <arpa/inet.h> // inet_ntoa()

// #include <pthread.h>

#elif defined _WIN32
#endif

/* Static */

/* Private */

void Server::acceptLoop() {
	struct sockaddr_in remote_addr;
	socklen_t addr_len = sizeof(struct sockaddr_in);
	int fd;

	while(this->isOpen()) {
		fd = accept(connexion_fd, (struct sockaddr*) &remote_addr, &addr_len);
		if(fd == -1) {
			nonfatal("Accept new connexion failed");
		} else {
			verbose_info(
					"Got connexion from "
					+ std::string(inet_ntoa(remote_addr.sin_addr))
					+ " port "
					+ std::to_string(ntohs(remote_addr.sin_port))
					+ " on socket #"
					+ std::to_string(fd)
					);
			class Player * player = new Player(fd, "noname",
					Tile::defaultTile.getAspect());
			this->addPlayer(player);
			this->luawrapper->spawnScript(player);
			if(player->getScreen() == nullptr) {
				warning("Spawn script didn't call spawn().");
				this->delPlayer(player->getId());
			}
		}

	}

	this->acceptThread = nullptr;
}

/* Public */

Server::Server() :
	// 0 is a valid file descriptor, but not a valid socket file descriptor.
	connexion_fd(0),
	port(0),
	acceptThread(nullptr),
	luawrapper(new Luawrapper(this))
{
	this->terminaisonLock.lock();
	// Main loop thread starting is done at _open(), not at constructor.
}

Server::~Server() {
	// Main loop thread stopping is done at _close(), not at destructor.
	if(this->isOpen()) {
		this->_close();
	}

	for(std::pair<std::string, Screen*> it : this->screens) {
		delete(it.second);
	}

	// A Tile must never be remove before the destruction of the server.
	for(std::pair<std::string, Tile*> it : this->tiles) {
		delete(it.second);
	}

	this->terminaisonLock.unlock();
}

void Server::_open(unsigned short port) {
	if(this->connexion_fd != 0) {
		this->_close();
	}
	this->port = port;

	// Open connexion.
#ifdef __linux__
	int sockfd;
	struct sockaddr_in addr;
	int yes=1;

	if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		nonfatal("Unable to create socket");
		return;
	}
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		nonfatal("Unable to set socket option : SO_REUSEADDR");
		return;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = 0;
	// memset(&(host_addr.sin_zero), '\0', 8); // FIXME: is this really useless ?
	if(bind(sockfd,(struct sockaddr *) &addr, sizeof(struct sockaddr)) == -1) {
		nonfatal("Unable to bind socket to port");
		return;
	}
	if(listen(sockfd, MAX_SOCKET_QUEUE) == -1) {
		nonfatal("Unable to listen on socket");
		return;
	}

	this->connexion_fd = sockfd;

	// Make asynchronous.
	// fcntl(servsock, F_SETFL, fcntl(servsock, F_GETFL) | O_ASYNC);

	// Ask the kernel to send us SIGIO on new connexion.
	// fcntl(servsock, F_SETOWN, getpid());

#elif defined _WIN32
#endif

	verbose_info("Server opened.");
	// TODO : IPv4/IPv6 connexions.

	// Start connexion accepting thread.
	this->acceptThread = new std::thread(&Server::acceptLoop, this);
}

void Server::_close() {
#ifdef __linux__
	close(this->connexion_fd);
#elif defined _WIN32
#endif
	this->connexion_fd = 0;
	this->port = 0;

	if(this->acceptThread != nullptr) {
		this->acceptThread->detach();
		delete(this->acceptThread);
		// this->acceptThread->join();
		this->acceptThread = nullptr;
	}

	verbose_info("Server closed.");
}

bool Server::isOpen() {
	return(this->connexion_fd != 0);
}

unsigned short Server::getPort() {
	if(this->isOpen()) {
		return(this->port);
	} else {
		return(0);
	}
}

// Automatically done by new Screen().
void Server::addScreen(std::string id, class Screen * screen) {
	if(this->getScreen(id) != nullptr) {
		this->delScreen(id);
		verbose_info("Screen '"+id+"' replaced.");
	}
	this->screens[id] = screen;
}

class Screen * Server::getScreen(std::string id) {
	return(this->screens[id]);
}

void Server::delScreen(std::string id) {
	if(this->screens[id] != nullptr) {
		delete(this->screens[id]);
		this->screens.erase(id);
	}
}

void Server::addTile(class Tile * tile) {
	if(this->tiles[tile->getId()] != nullptr) {
		delete(this->tiles[tile->getId()]);
		this->tiles.erase(tile->getId());
		verbose_info("Tile '"+tile->getId()+"' replaced.");
	}
	this->tiles[tile->getId()] = tile;
// TODO : replace tile's content and delete new one when replacing a tile.
}

class Tile * Server::getTile(std::string id) {
	class Tile * tile = this->tiles[id];
	if(tile == nullptr) {
		// return(&Tile::defaultTile);
		return(nullptr);
	} else {
		return(tile);
	}
}

void Server::addPlayer(class Player * player) {
	int id = player->getId();
	if(this->players[id] != nullptr) {
		warning("Player '"+std::to_string(id)+"' replaced.");
		delete(this->players[id]);
	}
	this->players[id] = player;
}

class Player * Server::getPlayer(int id) {
	class Player * player = this->players[id];
	if(player == nullptr) {
		return(nullptr);
	} else {
		return(player);
	}
}

void Server::delPlayer(int id) {
	class Player * player = this->players[id];
	if(player == nullptr) {
		verbose_info("Player '"+std::to_string(id)+
				"' can't be deleted: doesn't exist.");
	} else {
		delete(player);
		this->remPlayer(id);
	}
}

void Server::remPlayer(int id) {
	this->players.erase(id);
}

void Server::addScript(std::string id, std::string * filename) {
	if(this->getScript(id) != nullptr) {
		this->delScript(id);
		verbose_info("Script '"+id+"' replaced.");
	}
	this->scripts[id] = filename;
}

std::string * Server::getScript(std::string id) {
	return(this->scripts[id]);
}

void Server::delScript(std::string id) {
	if(this->scripts[id] != nullptr) {
		delete(this->scripts[id]);
		this->scripts.erase(id);
	} else {
		verbose_info("Script '"+id+"' doesn't exist.");
	}
}

void Server::exeScript(std::string id, class Player * player, std::string arg) {
	std::string * script = this->getScript(id);
	if(script != nullptr) {
		this->luawrapper->executeFile(*script, player, arg);
	} else {
		verbose_info("Script '"+id+"' doesn't exist.");
	}
}

class Luawrapper * Server::getLua() {
	return(this->luawrapper);
}

void Server::waitForTerminaison() {
	this->terminaisonLock.lock();
}

