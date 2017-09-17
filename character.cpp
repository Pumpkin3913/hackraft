#include "character.h"

#include "zone.h"
#include "server.h"
#include "gauge.h"
#include "luawrapper.h"
#include "place.h"
#include "log.h"

#ifdef __linux__
#include <unistd.h>
#elif defined _WIN32
#endif

// PRIVATE

void Character::send(std::string message) {
	if(this->fd) {
		std::string toSend = message + "\n";
#ifdef __linux__
		write(this->fd, toSend.c_str(), toSend.length());
		// this->socket << message << std::endl;
#elif defined _WIN32
#endif
	}
}

std::string Character::receive() {
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

void Character::_close() {
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

void Character::loopFunction() {
	while(!this->stop) {
		this->parse();
	}
	this->loopThread = nullptr;
	delete(this);
}

void Character::parse() {
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
				this->zone->getServer()->doAction(cmd.substr(1), *this, arg);
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
				this->zone->event(this->getName().toString()+": "+arg);
			}
		} else if(cmd == "quit") {
			this->stop = true;
		}
	}
}

// PUBLIC

Character::Character(
	Uuid id,
	int fd,
	Name name,
	const Aspect& aspect
) :
	Aspected(aspect),
	Named(name),
	fd(fd),
	id(id),
	zone(nullptr),
	x(0),
	y(0),
	whenDeath(),
	ghost(false),
/*
	movepoints(0),
	visible(true),
	movable(true),
*/
	loopThread(nullptr),
	stop(false)
{ }

Character::~Character() {
	info("Character "+this->getId().toString()+" deleted.");
	if(this->zone) {
		if(this->whenDeath != Script::noValue) {
			Script script = this->whenDeath;
			this->whenDeath = Script::noValue;
			script.execute(*(this->zone->getServer()->getLua()), this);
		}
		this->zone->exitCharacter(this);
		this->zone->getServer()->remCharacter(id);
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

void Character::spawn(class Zone * zone, int x, int y) {
	if(!this->loopThread) {
		this->zone = zone;
		this->zone->enterCharacter(this, x, y);
		this->loopThread = new std::thread(&Character::loopFunction, this);
		this->follow(this);
		info("Character "+this->getId().toString()+" spawn successfully.");
	}
}

Uuid Character::getId() {
	return(this->id);
}

/* XXX //
// Override Aspected::setAspect();
// Currently done by luawrapper.cpp .
void Character::setAspect(Aspect aspect) {
	// (Aspected*) (this)->setAspect(aspect);
	this->Aspected::setAspect(aspect); // XXX
	this->zone.updateCharacter(this);
}
// XXX */

class Zone * Character::getZone() {
	return(this->zone);
}

unsigned int Character::getX() {
	return(this->x);
}

unsigned int Character::getY() {
	return(this->y);
}

void Character::setXY(int x, int y) {
	this->x = x;
	this->y = y;
	if(this->zone) {
		this->zone->updateCharacter(this);
	}
}

void Character::move(int xShift, int yShift) {
	int new_x = this->x + xShift;
	int new_y = this->y + yShift;
	if(this->zone and new_x >= 0 and new_y >= 0
			and this->zone->isPlaceValid((unsigned) new_x, (unsigned) new_y)) {
		if(this->ghost or this->zone->canLandCharacter(this, new_x, new_y)) {
			this->setXY(new_x, new_y);

			// Trigger landon script.
			class Place * place = this->zone->getPlace(new_x, new_y);
			place->getWhenWalkedOn().execute(*(this->zone->getServer()->getLua()), this);
		}
	}
}

void Character::changeZone(class Zone * newZone, int x, int y) {
	if(this->zone) {
		this->zone->exitCharacter(this);
		this->zone = newZone;
		this->zone->enterCharacter(this, x, y);
	}
}

const Script& Character::getWhenDeath() {
	return(this->whenDeath);
}

void Character::setWhenDeath(const Script& script) {
	this->whenDeath = script;
}

class Gauge * Character::getGauge(const Name& name) {
	try {
		return(this->gauges.at(name.toString()));
	} catch(...) {
		return(nullptr);
	}
}

void Character::addGauge(class Gauge * gauge) {
	class Gauge * old = this->getGauge(gauge->getName());
	if(old) {
		delete(old);
	}
	this->gauges[gauge->getName().toString()] = gauge;
}

void Character::delGauge(const Name& name) {
	class Gauge * old = this->getGauge(name);
	if(old) {
		delete(old);
		this->gauges.erase(name.toString());
	} else {
		info("Gauge '"+name.toString()+"' doesn't exist: can't be deleted.");
	}
}

bool Character::isGhost() {
	return(this->ghost);
}

void Character::setGhost() {
	this->ghost = true;
}

void Character::setNotGhost() {
	this->ghost = false;
}

/*

unsigned int Character::getMovePoints() {
	return(this->movepoints);
}

void Character::setMovePoints(unsigned int points) {
	this->movepoints = points;
}

void Character::resetMovePoints() {
	this->movepoints = 0;
}

bool Character::isVisible() {
	return(this->visible);
}

void Character::setVisible() {
	this->visible = true;
}

void Character::setNotVisible() {
	this->visible = false;
}

bool Character::isMovable() {
	return(this->movable);
}

void Character::setMovable() {
	this->movable = true;
}

void Character::setNotMovable() {
	this->movable = false;
}

*/

/* Send messages to client */

void Character::message(std::string message) {
	this->send("msg " + message);
}

void Character::updateCharacter(class Character * character) {
	// move <plrID> <X> <Y>
	this->send(
			"move "
			+ character->getId().toString()
			+ " "
			+ std::to_string(Aspect::getAspectEntry(character->getAspect()))
			+ " "
			+ std::to_string(character->getX())
			+ " "
			+ std::to_string(character->getY())
		  );
}

void Character::updateCharacterExit(class Character * character) {
	this->send("exit "+character->getId().toString());
}

void Character::updateFloor() {
	this->send("zonename " + this->zone->getName().toString());
	// floor <W> <H> <name>
	this->send(
			"zone "
			+ std::to_string(this->zone->getWidth())
			+ " "
			+ std::to_string(this->zone->getHeight())
			+ " "
			+ this->zone->getName().toString()
		  );

	// Send places' aspects.
	std::string toSend = "";
	for(unsigned int y=0; y<this->zone->getHeight(); y++) {
		for(unsigned int x=0; x<this->zone->getWidth(); x++) {
			toSend += std::to_string(this->zone->getPlace(x, y)->getAspect().toEntry());
			toSend += ",";
		}
	}
	toSend.pop_back();
	this->send(toSend);
}

void Character::updateFloor(unsigned int x, unsigned int y, const Aspect& aspect) {
	// floorchange <aspect> <X> <Y>
	this->send(
			"floorchange "
			+ std::to_string(aspect.toEntry())
			+ " "
			+ std::to_string(x)
			+ " "
			+ std::to_string(y)
		  );
}

void Character::updateGauge(
	std::string name,
	unsigned int val,
	unsigned int max,
	const Aspect& full,
	const Aspect& empty
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
			+ std::to_string(Aspect::getAspectEntry(full))
			+ " "
			+ std::to_string(Aspect::getAspectEntry(empty))
	);
}

void Character::updateNoGauge(std::string name) {
	this->send(
			"nogauge "
			+ name
	);
}

/* XXX //
void Character::updateInventory(unsigned long int id, Aspect aspect) {
	// invent <id> <aspect>
	this->send(
			"invent "
			+ std::to_string(id)
			+ " "
			+ std::to_string(aspect)
	);
}

void Character::updateNoInventory(unsigned long int id) {
	// noinvent <id>
	this->send("noinvent " + std::to_string(id));
}

void Character::addPickupList(unsigned long int id, Aspect aspect) {
	// pickuplist <id> <aspect>
	this->send(
			"addpickuplist "
			+ std::to_string(id)
			+ " "
			+ std::to_string(aspect)
	);
}

void Character::remPickupList(unsigned long int id) {
	// pickuplist <id> <aspect>
	this->send(
			"rempickuplist "
			+ std::to_string(id)
	);
}
// XXX */

void Character::follow(class Character * character) {
	this->send("follow " + character->getId().toString());
}

void Character::hint(Aspect aspect, std::string hint) {
	this->send("hint "
		+ std::to_string(aspect.toEntry())
		+ " "
		+ hint
	);
}
