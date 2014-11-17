#include "screen.h"

#include "server.h"
#include "tile.h"
#include "place.h"
#include "player.h"
#include "object.h"

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

bool Screen::isPlaceValid(unsigned int x, unsigned int y) {
	return(x >= 0 && x < this->width && y >= 0 && y < this->height);
}

class Tile * Screen::getTile(int x, int y) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		return(place->getTile());
	} else {
		return(NULL);
	}
}

void Screen::setTile(int x, int y, class Tile * tile) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		place->setTile(tile);
		this->updateTile(x, y);
	}
}

class Object * Screen::getTopObject(int x, int y) {
	class Place * place = this->getPlace(x,y);
	if(place && place->getObjects()->size() > 0) {
		return(place->getObjects()->front());
	} else {
		return(NULL);
	}
}

class Object * Screen::getObject(int x, int y, unsigned long int id) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		for(class Object * object : *(place->getObjects())) {
			if(object->getId() == id) {
				return(object);
			}
		}
		return(NULL);
	} else {
		return(NULL);
	}
}

// Only called by Player.
const std::list<class Object *> * Screen::getObjectList(int x, int y) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		return(place->getObjects());
	} else {
		return(NULL);
	}
}

void Screen::addObject(int x, int y, class Object * object) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		place->getObjects()->push_front(object);
		this->updateObject(x, y);
		for(int id : this->players) {
			class Player * player = this->getPlayer(id);
			if(player && player->getX() == x && player->getY() == y) {
				player->addPickupList(object->getId(), object->getAspect());
			}
		}
	}
}

void Screen::remObject(int x, int y, unsigned long int id) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		std::list<class Object *> * lst = place->getObjects();
		for(auto it = lst->begin(); it != lst->end(); it++) {
			if((*it)->getId() == id) {
				lst->erase(it);
				this->updateObject(x, y); // FIXME: only if top changed.
				for(int player_id : this->players) {
					class Player * player = this->getPlayer(player_id);
					if(player && player->getX() == x && player->getY() == y) {
						player->remPickupList(id);
					}
				}
				break;
			}
		}
	}
}

std::string * Screen::getLandOn(int x, int y) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		return(place->getLandOn());
	} else {
		return(NULL);
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

/* Called by Player only */

bool Screen::canLandPlayer(class Player * player, int x, int y) {
	class Place * place;
	if(this->isPlaceValid(x,y) && (place = this->getPlace(x,y)) != NULL) {
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
	for(unsigned int x=0; x<this->width; x++) {
		for(unsigned int y=0; y<this->height; y++) {
			class Object * object = this->getTopObject(x, y);
			if(object) {
				player->updateObject(x, y, object->getAspect());
			}
		}
	}
	for(class Object * object :
			*(this->getObjectList(player->getX(), player->getY()))) {
		player->addPickupList(object->getId(), object->getAspect());
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
	if(player != NULL) {
		return(player);
	} else {
		this->players.remove(id);
		return(NULL);
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
		return(NULL);
	}
}

void Screen::updateObject(int x, int y) {
	class Object * object = this->getTopObject(x, y);
	if(object) {
		for(int id : this->players) {
			class Player * player = this->getPlayer(id);
			if(player) player->updateObject(x, y, object->getAspect());
		}
	} else {
		for(int id : this->players) {
			class Player * player = this->getPlayer(id);
			if(player) player->updateNoObject(x, y);
		}
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

