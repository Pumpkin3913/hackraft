#include "server.h"

#include "player.h"
#include "zone.h"
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
			class Player * player = new Player(fd, Name{}, Aspect{});
			this->addPlayer(player);
			this->luawrapper->spawnScript(player);
			if(player->getZone() == nullptr) {
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

	for(std::pair<std::string, Zone*> it : this->zones) {
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

// Automatically done by new Zone().
void Server::addZone(std::string id, class Zone * zone) {
	if(this->getZone(id) != nullptr) {
		this->delZone(id);
		verbose_info("Zone '"+id+"' replaced.");
	}
	this->zones[id] = zone;
}

class Zone * Server::getZone(std::string id) {
	return(this->zones[id]);
}

void Server::delZone(std::string id) {
	if(this->zones[id] != nullptr) {
		delete(this->zones[id]);
		this->zones.erase(id);
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

void Server::addAction(const Script& script, std::string trigger) {
	if(this->actions.count(trigger) > 0) {
		warning("Action '"+trigger+"' replaced.");
	}
	this->actions[trigger] = script;
}

const Script& Server::getAction(std::string trigger) {
	try {
		return(this->actions.at(trigger));
	} catch (const std::out_of_range& oor) {
		return(Script::noValue);
	}
}

void Server::delAction(std::string trigger) {
	if(this->actions.count(trigger) == 0) {
		verbose_info("Action '"+trigger+"' can't be deleted: doesn't exist.");
	} else {
		this->actions.erase(trigger);
	}
}

void Server::doAction(std::string trigger, class Player& player, std::string arg) {
	try {
		this->actions.at(trigger).execute(*(this->luawrapper), &player, arg);
	} catch (const std::out_of_range& oor) {
		verbose_info("Action '"+trigger+"' doesn't exist.");
	}
}

class Luawrapper * Server::getLua() {
	return(this->luawrapper);
}

void Server::waitForTerminaison() {
	this->terminaisonLock.lock();
}
