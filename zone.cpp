#include "zone.h"

#include "aspect.h"
#include "place.h"
#include "player.h"
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
	this->places = std::vector<class Place>(width * height, Place(*this, base_aspect));
	this->server->addZone(id, this); // XXX ??
}

Zone::~Zone() {
	// For now, players in a zone are deleted with it.
	for(Uuid id : this->players) {
		this->server->delPlayer(id);
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
	for(Uuid id : this->players) {
		class Player * player = this->getPlayer(id);
		if(player) player->message(message);
	}
}

/* Called by Player only */

bool Zone::canLandPlayer(class Player * player, int x, int y) {
	class Place * place;
	if(this->isPlaceValid(x,y) && (place = this->getPlace(x,y)) != nullptr) {
		return(place->isWalkable());
	} else {
		return(false);
	}
}

void Zone::enterPlayer(class Player * player, int x, int y) {
	this->players.push_front(player->getId());
	player->setXY(x, y);
	player->updateFloor();
	this->updatePlayer(player);
	for(Uuid id : this->players) {
		if(id != player->getId()) {
			class Player * p = this->getPlayer(id);
			if(p) player->updatePlayer(p);
		}
	}
}

void Zone::exitPlayer(class Player * player) {
	this->players.remove(player->getId());
	for(Uuid id : this->players) {
		class Player * p = this->getPlayer(id);
		if(p) p->updatePlayerExit(player);
	}
}

void Zone::updatePlayer(class Player * player) {
	for(Uuid id : this->players) {
		class Player * p = this->getPlayer(id);
		if(p) p->updatePlayer(player);
	}
}

/* Private */

class Player * Zone::getPlayer(Uuid id) {
	class Player * player = this->server->getPlayer(id);
	if(player != nullptr) {
		return(player);
	} else {
		this->players.remove(id);
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
		for(Uuid id : this->players) {
			class Player * player = this->getPlayer(id);
			if(player) player->updateFloor(x, y, aspect);
		}
	}
}

