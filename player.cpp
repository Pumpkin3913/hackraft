#include "player.h"

#include "zone.h"
#include "tile.h"
#include "server.h"
#include "gauge.h"
#include "luawrapper.h"

#ifdef __linux__
#include <unistd.h>
#elif defined _WIN32
#endif

// PRIVATE

void Player::send(std::string message) {
	if(this->fd) {
		std::string toSend = message + "\n";
#ifdef __linux__
		write(this->fd, toSend.c_str(), toSend.length());
		// this->socket << message << std::endl;
#elif defined _WIN32
#endif
	}
}

std::string Player::receive() {
	std::string msg = "";
#ifdef __linux__
	char c;
	int flag;
	flag = read(this->fd, &c, 1);
	while(flag && c != '\n') {
		msg.push_back(c);
		flag = read(this->fd, &c, 1);
	}
	if(!flag) {
#ifdef __linux__
		close(this->fd);
#elif defined _WIN32
#endif
		this->fd = 0;
		this->stop = true;
	}
#elif defined _WIN32
#endif
	return(msg);
}

void Player::_close() {
	if(this->fd) {
		this->send("EOF");
#ifdef __linux__
		close(this->fd);
#elif defined _WIN32
#endif
		this->fd = 0;
		this->stop = true;
	}
}

void Player::loopFunction() {
	while(!this->stop) {
		this->parse();
	}
	this->loopThread = nullptr;
	delete(this);
}

void Player::parse() {
	std::string msg;
	std::string cmd;
	std::string arg;
	std::size_t separator;

	msg = this->receive();
	if(!this->stop) {
		separator = msg.find_first_of(' ');
		if(separator == std::string::npos) {
			cmd = msg;
			arg = "";
		} else {
			cmd = msg.substr(0, separator);
			arg = msg.substr(separator+1); // from separator+1 to the end.
		}

		if(cmd[0] == '/') {
			if(this->zone) {
				this->zone->getServer()->exeScript(cmd.substr(1), this, arg);
			}
		} else if(cmd == "move") {
			signed int xShift = 0;
			signed int yShift = 0;
			bool valid = true;
			if(arg == "north") {
				yShift--;
			} else if(arg == "south") {
				yShift++;
			} else if(arg == "west") {
				xShift--;
			} else if(arg == "east") {
				xShift++;
			} else {
				valid = false;
			}
			if(valid) {
				this->move(xShift, yShift);
			}
		} else if(cmd == "say") {
			if(this->zone) {
				// this->zone->event(this->getName()+" say "+arg);
				// this->zone->event((const std::string)(this->getName())+" say "+arg); // XXX
				this->zone->event(this->getName().toString()+" say "+arg); // XXX
				// this->zone->event(this->getName()+" say "+arg); // XXX
				// this->zone->event(std::string{}+this->getName()+" say "+arg); // XXX
			}
		} else if(cmd == "quit") {
			this->stop = true;
		}
	}
}

// PUBLIC

Player::Player(
	int fd,
	Name name,
	Aspect aspect
) :
	Named(name), // XXX
	fd(fd),
	id(fd),
	// name(name), // XXX
	aspect(aspect),
	zone(nullptr),
	x(0),
	y(0),
	onDeath(""),
	ghost(false),
/*
	movepoints(0),
	visible(true),
	movable(true),
*/
	loopThread(nullptr),
	stop(false)
{ }

Player::~Player() {
	verbose_info("Player "+std::to_string(this->getId())+" deleted.");
	if(this->zone) {
		if(this->onDeath != "") {
			std::string script = this->onDeath;
			this->onDeath = "";
			this->zone->getServer()->getLua()->executeFile(script, this);
		}
		this->zone->exitPlayer(this);
		this->zone->getServer()->remPlayer(id);
	}
	for(auto it : this->gauges) {
		delete(it.second);
	}
	this->_close();
	if(this->loopThread != nullptr &&
			this->loopThread->get_id() != std::this_thread::get_id()) {
		// Only when deleted by something else than its own loopThread.
		this->loopThread->detach();
		delete(this->loopThread);
	}
}

void Player::spawn(class Zone * zone, int x, int y) {
	if(!this->loopThread) {
		this->zone = zone;
		this->zone->enterPlayer(this, x, y);
		this->loopThread = new std::thread(&Player::loopFunction, this);
		this->follow(this);
		verbose_info("Player "+std::to_string(this->getId())
				+" spawn successfully.");
	}
}

int Player::getId() {
	return(this->id);
}

Aspect Player::getAspect() {
	return(this->aspect);
}

void Player::setAspect(Aspect aspect) {
	this->aspect = aspect;
	if(this->zone) {
		this->zone->updatePlayer(this);
	}
}

class Zone * Player::getZone() {
	return(this->zone);
}

unsigned int Player::getX() {
	return(this->x);
}

unsigned int Player::getY() {
	return(this->y);
}

void Player::setXY(int x, int y) {
	this->x = x;
	this->y = y;
	if(this->zone) {
		this->zone->updatePlayer(this);
	}
}

void Player::move(int xShift, int yShift) {
	int new_x = this->x + xShift;
	int new_y = this->y + yShift;
	if(this->zone and new_x >= 0 and new_y >= 0
			and this->zone->isPlaceValid((unsigned) new_x, (unsigned) new_y)) {
		if(this->ghost or this->zone->canLandPlayer(this, new_x, new_y)) {
			this->setXY(new_x, new_y);
			// Trigger landon script.
			std::string * script =
				this->zone->getLandOn(new_x, new_y);
			if(script != nullptr) {
				this->zone->getServer()->getLua()->executeCode(*script, this);
			}
		}
	}
}

void Player::changeZone(class Zone * newZone, int x, int y) {
	if(this->zone) {
		this->zone->exitPlayer(this);
		this->zone = newZone;
		this->zone->enterPlayer(this, x, y);
	}
}

std::string Player::getOnDeath() {
	return(this->onDeath);
}

void Player::setOnDeath(std::string script) {
	this->onDeath = script;
}

class Gauge * Player::getGauge(const Name& name) {
	try {
		return(this->gauges.at(name.toString()));
	} catch(...) {
		return(nullptr);
	}
}

void Player::addGauge(class Gauge * gauge) {
	class Gauge * old = this->getGauge(gauge->getName());
	if(old) {
		delete(old);
	}
	this->gauges[gauge->getName().toString()] = gauge;
}

void Player::delGauge(const Name& name) {
	class Gauge * old = this->getGauge(name);
	if(old) {
		delete(old);
		this->gauges.erase(name.toString());
	} else {
		verbose_info("Gauge '"+name.toString()+"' doesn't exist: can't be deleted.");
	}
}

bool Player::isGhost() {
	return(this->ghost);
}

void Player::setGhost() {
	this->ghost = true;
}

void Player::setNotGhost() {
	this->ghost = false;
}

/*

unsigned int Player::getMovePoints() {
	return(this->movepoints);
}

void Player::setMovePoints(unsigned int points) {
	this->movepoints = points;
}

void Player::resetMovePoints() {
	this->movepoints = 0;
}

bool Player::isVisible() {
	return(this->visible);
}

void Player::setVisible() {
	this->visible = true;
}

void Player::setNotVisible() {
	this->visible = false;
}

bool Player::isMovable() {
	return(this->movable);
}

void Player::setMovable() {
	this->movable = true;
}

void Player::setNotMovable() {
	this->movable = false;
}

*/

/* Send messages to client */

void Player::message(std::string message) {
	this->send("msg " + message);
}

void Player::updatePlayer(class Player * player) {
	// move <plrID> <X> <Y>
	this->send(
			"move "
			+ std::to_string(player->getId())
			+ " "
			+ std::to_string(player->getAspect())
			+ " "
			+ std::to_string(player->getX())
			+ " "
			+ std::to_string(player->getY())
		  );
}

void Player::updatePlayerExit(class Player * player) {
	this->send("exit "+std::to_string(player->getId()));
}

void Player::updateFloor() {
	// floor <W> <H> <name>
	this->send(
			"floor "
			+ std::to_string(this->zone->getWidth())
			+ " "
			+ std::to_string(this->zone->getHeight())
			+ " "
			+ this->zone->getName().toString()
		  );

	// Send tiles.
	std::string toSend = "";
	for(unsigned int y=0; y<this->zone->getHeight(); y++) {
		for(unsigned int x=0; x<this->zone->getWidth(); x++) {
			toSend += std::to_string(this->zone->getTile(x, y)->getAspect());
			toSend += ",";
		}
	}
	toSend.pop_back();
	this->send(toSend);
}

void Player::updateTile(unsigned int x, unsigned int y, Aspect aspect) {
	// tilechange <aspect> <X> <Y>
	this->send(
			"tilechange "
			+ std::to_string(aspect)
			+ " "
			+ std::to_string(x)
			+ " "
			+ std::to_string(y)
		  );
}

void Player::updateGauge(
	std::string name,
	unsigned int val,
	unsigned int max,
	Aspect full,
	Aspect empty
) {
	// gauge <name> <val> <max> <full> <empty>
	this->send(
			"gauge "
			+ name
			+ " "
			+ std::to_string(val)
			+ " "
			+ std::to_string(max)
			+ " "
			+ std::to_string(full)
			+ " "
			+ std::to_string(empty)
	);
}

void Player::updateNoGauge(std::string name) {
	this->send(
			"nogauge "
			+ name
	);
}

void Player::updateInventory(unsigned long int id, Aspect aspect) {
	// invent <id> <aspect>
	this->send(
			"invent "
			+ std::to_string(id)
			+ " "
			+ std::to_string(aspect)
	);
}

void Player::updateNoInventory(unsigned long int id) {
	// noinvent <id>
	this->send("noinvent " + std::to_string(id));
}

void Player::addPickupList(unsigned long int id, Aspect aspect) {
	// pickuplist <id> <aspect>
	this->send(
			"addpickuplist "
			+ std::to_string(id)
			+ " "
			+ std::to_string(aspect)
	);
}

void Player::remPickupList(unsigned long int id) {
	// pickuplist <id> <aspect>
	this->send(
			"rempickuplist "
			+ std::to_string(id)
	);
}

void Player::follow(class Player * player) {
	this->send("follow " + std::to_string(player->getId()));
}

