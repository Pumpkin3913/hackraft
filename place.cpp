#include "place.h"

#include "aspect.h"

Place::Place(
	const Aspect& aspect,
	bool walkable
) :
	Aspected(aspect),
	walkable(walkable)
{ }

/* Walkable. */

bool Place::isWalkable() const {
	return(this->walkable);
}

void Place::setWalkable() {
	this->walkable = true;
}

void Place::setNotWalkable() {
	this->walkable = false;
}

/* When Walked On. */

const Script& Place::getWhenWalkedOn() const {
	return(this->whenWalkOn);
}

void Place::setWhenWalkedOn(const Script script) {
	this->whenWalkOn = script;
}

void Place::resetWhenWalkedOn() {
	this->whenWalkOn = Script{};
}
