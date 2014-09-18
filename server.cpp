#include "server.h"

#include "screen.h"
#include "aspect.h"
#include "tile.h"
#include "luawrapper.h"
#include "error.h"

#include <thread>
#include <cstring> // strerror()

#ifdef __linux__
#include <unistd.h> // close()
#include <sys/socket.h> // socket(), bind(), listen()
#include <netinet/in.h> // IPv4, IPv6
#include <arpa/inet.h> // inet_ntoa()

// #include <pthread.h>

#elif defined _WIN32
#endif

/* Static */

class Tile Server::defaultTile = Tile("nowhere", "Nowhere", "Nowhere...", (Aspect) 0);

/* Private */

void Server::acceptLoop() {
	class Player * player;
	struct sockaddr_in remote_addr;
	socklen_t addr_len = sizeof(struct sockaddr_in);
	int fd;

	while(!this->stop) {
		fd = accept(connexion_fd, (struct sockaddr*) &remote_addr, &addr_len);
		if(fd == -1) {
			nonfatal(
					"Accept new connexion failed: "
					+ std::string(strerror(errno))
					);
		} else {
			verbose_info(
					"Got connexion from "
					+ std::string(inet_ntoa(remote_addr.sin_addr))
					+ " port "
					+ std::to_string(ntohs(remote_addr.sin_port))
					+ " on socket #"
					+ std::to_string(fd)
					);
		}

		// player = new Player(fd, "noname", "nodescription", (Aspect) '@', this->spawnScreen); // XXX
		player = new Player(fd, "noname", "nodescription", (Aspect) '@');
		// TODO : Server::acceptLoop() : call spawn script.
	}
}

/* Public */

Server::Server() :
	connexion_fd(0), // 0 is a valid file descriptor, but not a valid socket file descriptor.
	port(0),
	spawnScreen(NULL),
	acceptThread(NULL),
	luawrapper(new Luawrapper(this)),
	stop(false)
{
// ToDO : Start main loop thread. Nope: done at _open().
}

Server::~Server() {
	if(this->isOpen()) {
		this->_close();
	}

	for(std::pair<const std::basic_string<char>, Screen*> it : this->screens) {
		delete(it.second);
	}

// TODO : Destroy tiles here. Never before.
// ToDO : Stop main loop thread. Nope: done at _close().

	info("Exiting...");
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

	// fcntl(servsock, F_SETFL, fcntl(servsock, F_GETFL) | O_ASYNC); // Make asynchronous. // XXX
	// fcntl(servsock, F_SETOWN, getpid()); // Ask the kernel to send us SIGIO on new connexion. // XXX

	verbose_info("Connexions set on.");
// ToDO : latter : IPv4/IPv6 connexions.
#elif defined _WIN32
#endif

	// Start connexion accepting thread.
#ifdef __linux__
	this->acceptThread = new std::thread(&Server::acceptLoop, this);
#elif defined _WIN32
#endif
}

void Server::_close() {
#ifdef __linux__
	close(this->connexion_fd);
#elif defined _WIN32
#endif
	if(this->acceptThread != NULL) {
		// Interrupt thread
		// delete(this->acceptThread); // XXX
		this->stop = true;
		this->acceptThread->join();
		this->acceptThread = NULL;
	}
	this->connexion_fd = 0;
	this->port = 0;
}

bool Server::isOpen() {
	return(this->connexion_fd == 0);
}

unsigned short Server::getPort() {
	if(this->isOpen()) {
		return(this->port);
	} else {
		return(0);
	}
}

void Server::addScreen(std::string id, class Screen * screen) {
	this->screens[id] = screen;
}

class Screen * Server::getScreen(std::string id) {
	return(this->screens[id]);
}

void Server::delScreen(std::string id) {
	if(this->screens[id] != NULL) {
		delete(this->screens[id]);
		this->screens.erase(id);
	}
}

void Server::addTile(class Tile * tile) {
	if(this->tiles[tile->getId()] != NULL) {
		delete(this->tiles[tile->getId()]);
		this->tiles.erase(tile->getId());
	}
	this->tiles[tile->getId()] = tile;
}

class Tile * Server::getTile(std::string id) {
	class Tile * tile = this->tiles[id];
	if(tile == NULL) {
		return(&Server::defaultTile);
	} else {
		return(tile);
	}
}

