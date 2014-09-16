#include "tile.h"

Tile::Tile(std::string id, std::string name, std::string description, Aspect aspect) :
	id(id),
	name(name),
	description(description),
	aspect(aspect),
	_canLand(true)
{
}

Tile::Tile(std::string id, std::string name, std::string description, Aspect aspect, bool canLand) :
	id(id),
	name(name),
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

std::string Tile::getName() {
	return(this->name);
}

void Tile::setName(std::string name) {
	this->name = name;
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

