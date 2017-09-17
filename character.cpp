#include "character.h"

#include "player.h"
#include "zone.h"
#include "server.h"
#include "gauge.h"
#include "luawrapper.h"
#include "place.h"
#include "log.h"

// PUBLIC

Character::Character(
	Uuid id,
	Name name,
	const Aspect& aspect
) :
	Aspected(aspect),
	Named(name),
	player(nullptr),
	id(id),
	zone(nullptr),
	x(0),
	y(0),
	whenDeath(),
	ghost(false)
/*
	movepoints(0),
	visible(true),
	movable(true),
*/
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
}

void Character::spawn(class Zone * zone, int x, int y) {
	if(this->player) {
		this->player->spawn();
	}
	this->zone = zone;
	this->zone->enterCharacter(this, x, y);
	this->follow(this);
	info("Character "+this->getId().toString()+" spawn successfully.");
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

void Character::set_player(class Player * player) {
	this->player = player;
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
	if(this->player) {
		this->player->message(message);
	}
}

void Character::updateCharacter(class Character * character) {
	if(this->player) {
		this->player->updateCharacter(character);
	}
}

void Character::updateCharacterExit(class Character * character) {
	if(this->player) {
		this->player->updateCharacterExit(character);
	}
}

void Character::updateFloor() {
	if(this->player) {
		this->player->updateFloor();
	}
}

void Character::updateFloor(unsigned int x, unsigned int y, const Aspect& aspect) {
	if(this->player) {
		this->player->updateFloor(x, y, aspect);
	}
}

void Character::updateGauge(
	std::string name,
	unsigned int val,
	unsigned int max,
	const Aspect& full,
	const Aspect& empty
) {
	if(this->player) {
		this->player->updateGauge(name, val, max, full, empty);
	}
}

void Character::updateNoGauge(std::string name) {
	if(this->player) {
		this->player->updateNoGauge(name);
	}
}

void Character::follow(class Character * character) {
	if(this->player) {
		this->player->follow(character);
	}
}

void Character::hint(Aspect aspect, std::string hint) {
	if(this->player) {
		this->player->hint(aspect, hint);
	}
}
