#include "place.h"

#include "tile.h"

Place::Place(class Tile * tile) :
	tile(tile)
{
	this->local_description = "";
	this->local_aspect = noAspect;
	this->useLocalCanLand = false;
	this->local_canLand = true; // useless.
}

Place::~Place() {
}

class Tile * Place::getTile() {
	return(this->tile);
}

void Place::setTile(class Tile * tile) {
	this->tile = tile;
}

std::string Place::getDescription() {
	if(this->local_description == "") {
		return(this->tile->getDescription());
	} else {
		return(this->local_description);
	}
}

void Place::setDescription(std::string description) {
	this->local_description = description;
}

void Place::resetDescription() {
	this->local_description = "";
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

std::string Place::getLandon() {
	return(this->landon);
}

void Place::setLandon(std::string script) {
	this->landon = script;
}

void Place::resetLandon() {
	this->landon = "";
}

// getObject();
// addObject();
// remObject();
// getUsage();
// addUsage();
// remUsage();
// setLandon();
// resetLandon();

