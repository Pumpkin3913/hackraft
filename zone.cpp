#include "zone.h"

#include "aspect.h"
#include "place.h"
#include "character.h"
#include "server.h"
#include "log.h"

// TODO : Zone::setName() : broadcast new name.

Zone::Zone(
	class Server * server,
	std::string id,
	const Name& name,
	unsigned int width,
	unsigned int height,
	const Aspect& base_aspect
) :
	Named(name),
	server(server),
	id(id),
	width(width),
	height(height)
{
	this->places = std::vector<class Place>(width * height, Place(base_aspect));
	this->server->addZone(id, this); // XXX ??
}

Zone::~Zone() {
	// For now, characters in a zone are deleted with it.
	for(Uuid id : this->characters) {
		this->server->delCharacter(id);
	}
	info("Zone '"+id+"' deleted.");
}

class Server * Zone::getServer() {
	return(this->server);
}

std::string Zone::getId() {
	return(this->id);
}

unsigned int Zone::getWidth() {
	return(this->width);
}

unsigned int Zone::getHeight() {
	return(this->height);
}

bool Zone::isPlaceValid(int x, int y) {
	return(x >= 0 && x < this->width && y >= 0 && y < this->height);
}

void Zone::event(std::string message) {
	for(Uuid id : this->characters) {
		class Character * character = this->getCharacter(id);
		if(character) character->message(message);
	}
}

/* Called by Character only */

bool Zone::canLandCharacter(class Character * character, int x, int y) {
	class Place * place;
	if(this->isPlaceValid(x,y) && (place = this->getPlace(x,y)) != nullptr) {
		return(place->isWalkable());
	} else {
		return(false);
	}
}

void Zone::enterCharacter(class Character * character, int x, int y) {
	this->characters.push_front(character->getId());
	character->setXY(x, y);
	character->updateFloor();
	this->updateCharacter(character);
	for(Uuid id : this->characters) {
		if(id != character->getId()) {
			class Character * p = this->getCharacter(id);
			if(p) character->updateCharacter(p);
		}
	}
}

void Zone::exitCharacter(class Character * character) {
	this->characters.remove(character->getId());
	for(Uuid id : this->characters) {
		class Character * p = this->getCharacter(id);
		if(p) p->updateCharacterExit(character);
	}
}

void Zone::updateCharacter(class Character * character) {
	for(Uuid id : this->characters) {
		class Character * p = this->getCharacter(id);
		if(p) p->updateCharacter(character);
	}
}

/* Private */

class Character * Zone::getCharacter(Uuid id) {
	class Character * character = this->server->getCharacter(id);
	if(character != nullptr) {
		return(character);
	} else {
		this->characters.remove(id);
		return(nullptr);
	}
}

class Place * Zone::getPlace(int x, int y) {
	if(this->isPlaceValid(x,y)) {
		return(&(this->places[y*this->width+x]));
	} else {
		warning(
			"In zone "
			+ this->id
			+ ": invalid place "
			+ std::to_string(x)
			+ "-"
			+ std::to_string(y)
			+ "."
		);
		return(nullptr);
	}
}

void Zone::updatePlaceAspect(int x, int y) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		auto aspect = place->getAspect();
		for(Uuid id : this->characters) {
			class Character * character = this->getCharacter(id);
			if(character) character->updateFloor(x, y, aspect);
		}
	}
}

