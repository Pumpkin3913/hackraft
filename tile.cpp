#include "tile.h"

Tile::Tile(std::string id, std::string description, Aspect aspect, bool canLand) :
	id(id),
	description(description),
	aspect(aspect),
	_canLand(canLand)
{
}

Tile::~Tile() {
}

std::string Tile::getId() {
	return(this->id);
}

std::string Tile::getDescription() {
	return(this->description);
}

void Tile::setDescription(std::string description) {
	this->description = description;
}

Aspect Tile::getAspect() {
	return(this->aspect);
}

void Tile::setAspect(Aspect aspect) {
	this->aspect = aspect;
}

bool Tile::canLand() {
	return(this->_canLand);
}

void Tile::setCanLand() {
	this->_canLand = true;
}

void Tile::setCantLand() {
	this->_canLand = false;
}

