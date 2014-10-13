#include "tile.h"

class Tile Tile::defaultTile =
		Tile("nowhere", "Nowhere...", (Aspect) 0);

Tile::Tile(std::string id, std::string name, Aspect aspect, bool canLand) :
	id(id),
	name(name),
	aspect(aspect),
	_canLand(canLand)
{
}

Tile::~Tile() {
}

std::string Tile::getId() {
	return(this->id);
}

std::string Tile::getName() {
	return(this->name);
}

void Tile::setName(std::string name) {
	this->name = name;
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

