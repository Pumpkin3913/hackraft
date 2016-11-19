#include "zone.h"

#include "server.h"
#include "tile.h"
#include "place.h"
#include "player.h"

// TODO : Zone::setName() : broadcast new name.

Zone::Zone(
	class Server * server,
	std::string id,
	const Name& name,
	unsigned int width,
	unsigned int height,
	class Tile * baseTile
) :
	Named(name),
	server(server),
	id(id),
	width(width),
	height(height)
{
	this->places = std::vector<class Place>(width * height, Place(baseTile));
	this->server->addZone(id, this);
}

Zone::~Zone() {
	// For now, players in a zone are deleted with it.
	for(int id : this->players) {
		this->server->delPlayer(id);
	}
	verbose_info("Zone '"+id+"' deleted.");
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

class Tile * Zone::getTile(int x, int y) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		return(place->getTile());
	} else {
		return(nullptr);
	}
}

void Zone::setTile(int x, int y, class Tile * tile) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		place->setTile(tile);
		this->updateTile(x, y);
	}
}

std::string * Zone::getLandOn(int x, int y) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		return(place->getLandOn());
	} else {
		return(nullptr);
	}
}

void Zone::setLandOn(int x, int y, std::string script) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		place->setLandOn(script);
	} else {
		// Warning already done by Zone::getPlace();
	}
}

void Zone::resetLandOn(int x, int y) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		place->resetLandOn();
	} else {
		// Warning already done by Zone::getPlace();
	}
}

void Zone::event(std::string message) {
	for(int id : this->players) {
		class Player * player = this->getPlayer(id);
		if(player) player->message(message);
	}
}

/* XXX //
std::string Zone::getTag(int x, int y, std::string id) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		return(place->getTag(id));
	} else {
		return("");
	}
}

void Zone::setTag(int x, int y, std::string id, std::string value) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		place->setTag(id, value);
	} else {
		warning("In zone "
		+ this->id
		+ ": unable to set tag "
		+ id
		+ "to value "
		+ value
		+ " because place "
		+ std::to_string(x)
		+ "-"
		+ std::to_string(y)
		+ "is invalid.");
	}
}

void Zone::delTag(int x, int y, std::string id) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		place->delTag(id);
	} else {
		warning("In zone "
		+ this->id
		+ ": unable to remove tag "
		+ id
		+ " because place "
		+ std::to_string(x)
		+ "-"
		+ std::to_string(y)
		+ "is invalid.");
	}
}
// XXX */

/* Called by Player only */

bool Zone::canLandPlayer(class Player * player, int x, int y) {
	class Place * place;
	if(this->isPlaceValid(x,y) && (place = this->getPlace(x,y)) != nullptr) {
		return(place->canLand());
	} else {
		return(false);
	}
}

void Zone::enterPlayer(class Player * player, int x, int y) {
	this->players.push_front(player->getId());
	player->setXY(x, y);
	player->updateFloor();
	this->updatePlayer(player);
	for(int id : this->players) {
		if(id != player->getId()) {
			class Player * p = this->getPlayer(id);
			if(p) player->updatePlayer(p);
		}
	}
}

void Zone::exitPlayer(class Player * player) {
	this->players.remove(player->getId());
	for(int id : this->players) {
		class Player * p = this->getPlayer(id);
		if(p) p->updatePlayerExit(player);
	}
}

void Zone::updatePlayer(class Player * player) {
	for(int id : this->players) {
		class Player * p = this->getPlayer(id);
		if(p) p->updatePlayer(player);
	}
}

/* Private */

class Player * Zone::getPlayer(int id) {
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

void Zone::updateTile(int x, int y) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		Aspect aspect = place->getAspect();
		for(int id : this->players) {
			class Player * player = this->getPlayer(id);
			if(player) player->updateTile(x, y, aspect);
		}
	}
}

