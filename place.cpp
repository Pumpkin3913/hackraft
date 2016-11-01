#include "place.h"

#include "tile.h"

Place::Place(class Tile * tile) :
	tile(tile),
	local_name(""),
	local_aspect(noAspect),
	useLocalCanLand(false),
	local_canLand(true), // useless.
	landon(nullptr)
{ }

Place::~Place() {
}

class Tile * Place::getTile() {
	return(this->tile);
}

void Place::setTile(class Tile * tile) {
	this->tile = tile;
}

std::string Place::getName() {
	if(this->local_name == "") {
		return(this->tile->getName());
	} else {
		return(this->local_name);
	}
}

void Place::setName(std::string name) {
	this->local_name = name;
}

void Place::resetName() {
	this->local_name = "";
}

Aspect Place::getAspect() {
	if(this->local_aspect == noAspect) {
		return(this->tile->getAspect());
	} else {
		return(this->local_aspect);
	}
}

void Place::setAspect(Aspect aspect) {
	this->local_aspect = aspect;
}

void Place::resetAspect() {
	this->local_aspect = noAspect;
}

bool Place::canLand() {
	if(this->useLocalCanLand) {
		return(this->local_canLand);
	} else {
		return(this->tile->canLand());
	}
}

void Place::setCanLand() {
	this->useLocalCanLand = true;
	this->local_canLand = true;
}

void Place::setCantLand() {
	this->useLocalCanLand = true;
	this->local_canLand = false;
}

void Place::resetCanLand() {
	this->useLocalCanLand = false;
}

std::list<class Object *> * Place::getObjects() {
	return(&(this->objects));
}

std::string * Place::getLandOn() {
	return(this->landon);
}

void Place::setLandOn(std::string script) {
	this->landon = new std::string(script);
}

void Place::resetLandOn() {
	if(this->landon) {
		delete(this->landon);
		this->landon = nullptr;
	}
}

