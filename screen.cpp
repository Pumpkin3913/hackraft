#include "screen.h"

#include "server.h"
#include "tile.h"
#include "place.h"
#include "player.h"
#include "object.h"

Screen::Screen(
	class Server * server,
	std::string name,
	unsigned int width,
	unsigned int height,
	class Tile * baseTile
) :
	server(server),
	name(name),
	width(width),
	height(height)
{
	this->places = std::vector<class Place>(width * height, Place(baseTile));
}

Screen::~Screen() {
	// For now, players in a screen are deleted with it.
	for(std::pair<int, Player*> it : this->players) {
		delete(it.second);
	}
}

class Server * Screen::getServer() {
	return(this->server);
}

std::string Screen::getName() {
	return(this->name);
}

void Screen::setName(std::string name) {
	this->name = name;
	// TODO : Screen::setName() : broadcast new name.
}

unsigned int Screen::getWidth() {
	return(this->width);
}

unsigned int Screen::getHeight() {
	return(this->height);
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
		return(place->setTile(tile));
	}
}

class Player * Screen::getPlayer(int id_fd) {
	try {
		return(this->players.at(id_fd));
	} catch(...) {
		return(NULL);
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
		for(std::pair<int, Player*> it : this->players) {
			if(it.second->getX() == x && it.second->getY() == y) {
				it.second->addFloorList(object->getId(), object->getAspect());
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
				for(std::pair<int, Player*> it : this->players) {
					if(it.second->getX() == x && it.second->getY() == y) {
						it.second->remFloorList(id);
					}
				}
				break;
			}
		}
	}
}

std::string Screen::getLandOn(int x, int y) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		return(place->getLandOn());
	} else {
		return("");
	}
}

void Screen::setLandOn(int x, int y, std::string script) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		place->setLandOn(script);
	}
}

void Screen::resetLandOn(int x, int y) {
	class Place * place = this->getPlace(x,y);
	if(place) {
		place->resetLandOn();
	}
}

void Screen::event(std::string message) {
	for(std::pair<int, Player*> it : this->players) {
		it.second->message(message);
	}
}

/* Called by Player only */

bool Screen::canLandPlayer(class Player * player, int x, int y) {
	class Tile * dst = this->getTile(x,y);
	if(dst != NULL) {
		return(dst->canLand());
	} else {
		return(false);
	}
}

void Screen::enterPlayer(class Player * player, int x, int y) {
	this->players[player->getId()] = player;
	player->setXY(x, y);
	player->updateFloor();
	this->updatePlayer(player);
	for(std::pair<int, Player*> it : this->players) {
		if(it.second != player) {
			player->updatePlayer(it.second);
		}
	}
	for(int x=0; x<this->width; x++) {
		for(int y=0; y<this->height; y++) {
			class Object * object = this->getTopObject(x, y);
			if(object) {
				player->updateObject(x, y, object->getAspect());
			}
		}
	}
	for(class Object * object :
			*(this->getObjectList(player->getX(), player->getY()))) {
		player->addFloorList(object->getId(), object->getAspect());
	}
}

void Screen::exitPlayer(class Player * player) {
	this->players.erase(player->getId());
	for(std::pair<int, Player*> it : this->players) {
		it.second->updatePlayerExit(player);
	}
}

void Screen::updatePlayer(class Player * player) {
	for(std::pair<int, Player*> it : this->players) {
		it.second->updatePlayer(player);
	}
}

/* Private */

class Place * Screen::getPlace(int x, int y) {
	if(x >= 0 && x < this->width && y >= 0 && y < this->height) {
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
		for(std::pair<int, Player*> it : this->players) {
			it.second->updateObject(x, y, object->getAspect());
		}
	} else {
		for(std::pair<int, Player*> it : this->players) {
			it.second->updateNoObject(x, y);
		}
	}
}

