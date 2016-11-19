#include "screen.h"

#include "server.h"
#include "tile.h"
#include "place.h"
#include "player.h"

Screen::Screen(
	class Server * server,
	std::string id,
	std::string name,
	unsigned int width,
	unsigned int height,
	class Tile * baseTile
) :
	server(server),
	id(id),
	name(name),
	width(width),
	height(height)
{
	this->places = std::vector<class Place>(width * height, Place(baseTile));
	this->server->addScreen(id, this);
}

Screen::~Screen() {
	// For now, players in a screen are deleted with it.
	for(int id : this->players) {
		this->server->delPlayer(id);
	}
	verbose_info("Screen '"+id+"' deleted.");
}

class Server * Screen::getServer() {
	return(this->server);
}

std::string Screen::getId() {
	return(this->id);
}

std::string Screen::getName() {
	return(this->name);
}

// TODO : Screen::setName() : broadcast new name.
void Screen::setName(std::string name) {
	this->name = name;
}

unsigned int Screen::getWidth() {
	return(this->width);
}

unsigned int Screen::getHeight() {
	return(this->height);
}

bool Screen::isPlaceValid(int x, int y) {
	return(x >= 0 && x < this->width && y >= 0 && y < this->height);
}

class Tile * Screen::getTile(int x, int y) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		return(place->getTile());
	} else {
		return(nullptr);
	}
}

void Screen::setTile(int x, int y, class Tile * tile) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		place->setTile(tile);
		this->updateTile(x, y);
	}
}

std::string * Screen::getLandOn(int x, int y) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		return(place->getLandOn());
	} else {
		return(nullptr);
	}
}

void Screen::setLandOn(int x, int y, std::string script) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		place->setLandOn(script);
	} else {
		// Warning already done by Screen::getPlace();
	}
}

void Screen::resetLandOn(int x, int y) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		place->resetLandOn();
	} else {
		// Warning already done by Screen::getPlace();
	}
}

void Screen::event(std::string message) {
	for(int id : this->players) {
		class Player * player = this->getPlayer(id);
		if(player) player->message(message);
	}
}

std::string Screen::getTag(int x, int y, std::string id) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		return(place->getTag(id));
	} else {
		return("");
	}
}

void Screen::setTag(int x, int y, std::string id, std::string value) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		place->setTag(id, value);
	} else {
		warning("In screen "
		+ this->name
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

void Screen::delTag(int x, int y, std::string id) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		place->delTag(id);
	} else {
		warning("In screen "
		+ this->name
		+ ": unable to remove tag "
		+ id
		+ " because place "
		+ std::to_string(x)
		+ "-"
		+ std::to_string(y)
		+ "is invalid.");
	}
}

/* Called by Player only */

bool Screen::canLandPlayer(class Player * player, int x, int y) {
	class Place * place;
	if(this->isPlaceValid(x,y) && (place = this->getPlace(x,y)) != nullptr) {
		return(place->canLand());
	} else {
		return(false);
	}
}

void Screen::enterPlayer(class Player * player, int x, int y) {
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

void Screen::exitPlayer(class Player * player) {
	this->players.remove(player->getId());
	for(int id : this->players) {
		class Player * p = this->getPlayer(id);
		if(p) p->updatePlayerExit(player);
	}
}

void Screen::updatePlayer(class Player * player) {
	for(int id : this->players) {
		class Player * p = this->getPlayer(id);
		if(p) p->updatePlayer(player);
	}
}

/* Private */

class Player * Screen::getPlayer(int id) {
	class Player * player = this->server->getPlayer(id);
	if(player != nullptr) {
		return(player);
	} else {
		this->players.remove(id);
		return(nullptr);
	}
}

class Place * Screen::getPlace(int x, int y) {
	if(this->isPlaceValid(x,y)) {
		return(&(this->places[y*this->width+x]));
	} else {
		warning(
			"In screen "
			+ this->name
			+ ": invalid place "
			+ std::to_string(x)
			+ "-"
			+ std::to_string(y)
			+ "."
		);
		return(nullptr);
	}
}

void Screen::updateTile(int x, int y) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		Aspect aspect = place->getAspect();
		for(int id : this->players) {
			class Player * player = this->getPlayer(id);
			if(player) player->updateTile(x, y, aspect);
		}
	}
}

