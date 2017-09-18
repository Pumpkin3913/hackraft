#include "server.h"

#include "character.h"
#include "zone.h"
#include "artifact.h"
#include "luawrapper.h"
#include "log.h"
#include "inventory.h"

#include <unistd.h> // close()
#include <sys/socket.h> // socket(), bind(), listen()
#include <netinet/in.h> // IPv4, IPv6
#include <arpa/inet.h> // inet_ntoa()
#include <fcntl.h> // fcntl()

#include <iostream>

/* Public */

Server::Server() {
	this->luawrapper = new Luawrapper(this);
}

Server::~Server() {
	if(this->isOpen()) {
		this->_close();
	}

	for(std::pair<std::string, Zone*> it : this->zones) {
		delete(it.second);
	}
}

void Server::_open(unsigned short port, const std::string& spawn_z, unsigned int spawn_x, unsigned int spawn_y) {
	if(this->connexion_fd != 0) {
		this->_close();
	}
	this->port = port;

	// Open connexion.
	int sockfd;
	struct sockaddr_in addr;
	int yes=1;

	if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		warning("Unable to create socket");
		return;
	}
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		warning("Unable to set socket option : SO_REUSEADDR");
		return;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = 0;
	// memset(&(host_addr.sin_zero), '\0', 8); // FIXME: is this really useless ?
	if(bind(sockfd,(struct sockaddr *) &addr, sizeof(struct sockaddr)) == -1) {
		warning("Unable to bind socket to port");
		return;
	}
	if(listen(sockfd, MAX_SOCKET_QUEUE) == -1) {
		warning("Unable to listen on socket");
		return;
	}

	fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL) | O_NONBLOCK); // Make non-blocking.

	this->connexion_fd = sockfd;

	this->spawn_zone = spawn_z;
	this->spawn_x = spawn_x;
	this->spawn_y = spawn_y;
	if(this->getZone(this->spawn_zone) == nullptr) {
		warning("Spawn zone not found: "+this->spawn_zone);
	}

	info("Server opened.");
	// TODO : IPv4/IPv6 connexions.
}

void Server::_close() {
	close(this->connexion_fd);
	this->connexion_fd = 0;
	this->port = 0;

	info("Server closed.");
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
		info("Zone '"+id+"' replaced.");
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

void Server::addCharacter(class Character * character) {
	Uuid id = character->getId();
	if(this->characters[id] != nullptr) {
		warning("Character '"+id.toString()+"' replaced.");
		delete(this->characters[id]);
	}
	this->characters[id] = character;
}

class Character * Server::getCharacter(Uuid id) {
	class Character * character = this->characters[id];
	if(character == nullptr) {
		return(nullptr);
	} else {
		return(character);
	}
}

void Server::delCharacter(Uuid id) {
	class Character * character = this->characters[id];
	if(character == nullptr) {
		info("Character '"+id.toString()+"' can't be deleted: doesn't exist.");
	} else {
		delete(character);
		this->remCharacter(id);
	}
}

void Server::remCharacter(Uuid id) {
	this->characters.erase(id);
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
		info("Action '"+trigger+"' can't be deleted: doesn't exist.");
	} else {
		this->actions.erase(trigger);
	}
}

void Server::doAction(std::string trigger, class Character& character, std::string arg) {
	try {
		this->actions.at(trigger).execute(*(this->luawrapper), &character, arg);
	} catch (const std::out_of_range& oor) {
		info("Action '"+trigger+"' doesn't exist.");
	}
}

Uuid Server::newArtifact(Name name) {
	Uuid id {};
	Artifact* artifact = new Artifact(name);
	this->artifacts[id] = artifact;
	return(id);
}

void Server::delArtifact(Uuid id) {
	Artifact* artifact = this->getArtifact(id);
	if(artifact != nullptr) {
		delete(artifact);
	}
	this->artifacts.erase(id);
}

class Artifact* Server::getArtifact(Uuid id) {
	try {
		return(this->artifacts.at(id));
	} catch(const std::out_of_range& oor) {
		return(nullptr);
	}
}

Uuid Server::newInventory(unsigned int size) {
	Uuid id {};
	Inventory* inventory = new Inventory(size);
	this->inventories[id] = inventory;
	return(id);
}

void Server::delInventory(Uuid id) {
	Inventory* inventory = this->getInventory(id);
	if(inventory != nullptr) {
		delete(inventory);
	}
	this->inventories.erase(id);
}

class Inventory* Server::getInventory(Uuid id) {
	try {
		return(this->inventories.at(id));
	} catch(...) {
		return(nullptr);
	}
}

Uuid Server::addTimer(unsigned int duration, const Script& script) {
	Uuid id {};
	this->timers.emplace(id, Timer{duration, script});
	return(id);
}

void Server::delTimer(Uuid id) {
	this->timers.erase(id);
}

void Server::triggerTimer(Uuid id) {
	auto it = this->timers.find(id);
	if(it == this->timers.end()) {
		warning("Cannot trigger timer: id not found.");
		return;
	}
	it->second.script.execute(*luawrapper);
	this->timers.erase(it);
}

unsigned int Server::getTimerRemaining(Uuid id) {
	auto it = this->timers.find(id);
	if(it == this->timers.end()) {
		return(0);
	}
	return(it->second.remaining);
}

void Server::setTimerRemaining(Uuid id, unsigned int remaining) {
	if(remaining == 0) {
		this->triggerTimer(id);
		return;
	}
	auto it = this->timers.find(id);
	if(it == this->timers.end()) {
		warning("Cannot set timer's remaining time: id not found.");
		return;
	}
	it->second.remaining = remaining;
}

class Luawrapper * Server::getLua() {
	return(this->luawrapper);
}

void Server::loop() {
	while(not this->stop) {
		this->check_connection();
		// this->check_console(); // TODO
		this->check_players();
		usleep(10000); // 0.01 sec.
		// std::this_thread::sleep_for(std::chrono::milliseconds(10));
		// TODO: when to step timers?
	}
}

/* Private */

void Server::check_connection() {
	struct sockaddr_in remote_addr;
	socklen_t addr_len = sizeof(struct sockaddr_in);
	int fd;

	fd = accept(connexion_fd, (struct sockaddr*) &remote_addr, &addr_len);
	if(fd == -1) {
		if(errno == EAGAIN or errno == EWOULDBLOCK) {
		} else {
			warning("Accept new connexion failed");
		}
		return;
	}

	// Make non-blocking.
	fcntl(fd, F_SETFL, O_NONBLOCK);

	info(
		"Got connexion from "
		+ std::string(inet_ntoa(remote_addr.sin_addr))
		+ " port "
		+ std::to_string(ntohs(remote_addr.sin_port))
		+ " on socket #"
		+ std::to_string(fd)
	);

	class Zone* spawn_z = this->getZone(this->spawn_zone);
	if(spawn_z == nullptr) {
		warning("Spawn zone not found : "+this->spawn_zone);
		close(fd);
		return;
	}

	Uuid id {};
	class Character * character = new Character(id, Name{}, Aspect{});
	this->addCharacter(character);

	class Player * player = new Player(fd, character);
	this->players.push_back(player);
	character->setPlayer(player);
	character->changeZone(spawn_z, spawn_x, spawn_y);
	this->luawrapper->spawnScript(character);
	player->follow(character);
}

void Server::check_console() {
	std::string input;
	std::getline(std::cin, input);
	if(input != "") {
		this->getLua()->executeCode(input);
	}
}

void Server::check_players() {
	auto player = this->players.begin();
	while(player != this->players.end()) {
		if((*player)->delme()) {
			delete(*player);
			player = this->players.erase(player);
		} else {
			(*player)->check_action();
			player++;
		}
	}
}

void Server::step_timers() {
	for(auto it = this->timers.begin(); it != this->timers.end(); ) {
		it->second.remaining--;
		if(it->second.remaining == 0) {
			Script script = std::move(it->second.script);
			it = this->timers.erase(it);
			script.execute(*luawrapper);
		} else {
			it++;
		}
	}
}
