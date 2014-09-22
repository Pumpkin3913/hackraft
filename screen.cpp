#include "screen.h"

#include "place.h"
#include "player.h"

Screen::Screen(std::string name, unsigned int width, unsigned int height, class Tile* defaultTile) :
	name(name),
	width(width),
	height(height)
{
	class Place defaultPlace(defaultTile);
	this->places = std::vector<class Place>(width * height, defaultPlace);
}

Screen::~Screen() {
	// For now, players in a screen are deleted with it.
	for(std::pair<int, Player*> it : this->players) {
		delete(it.second);
	}
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

class Place * Screen::getPlace(int x, int y) {
	if(x >= 0 && x < this->width && y >= 0 && y < this->height) {
		return(&(this->places[y*this->width+x]));
	} else {
		return(NULL);
	}
}

class Player * Screen::getPlayer(int id_fd) {
	return(this->players[id_fd]);
}

void Screen::event(std::string message) {
	for(std::pair<int, Player*> it : this->players) {
		it.second->message(message);
	}
}

/* Called by Player only */

bool Screen::canLandPlayer(class Player * player, int x, int y) {
	class Place * dst = this->getPlace(x,y);
	if(dst != NULL) {
		return(dst->canLand());
	} else {
		return(false);
	}
}

void Screen::enterPlayer(class Player * player) {
	this->players[player->getId()] = player;
	player->updateFloor();
	this->updatePlayerPosition(player);
	for(std::pair<int, Player*> it : this->players) {
		if(it.second != player) {
			player->updatePlayer(it.second);
		}
	}
}

void Screen::exitPlayer(class Player * player) {
	this->players.erase(player->getId());
	for(std::pair<int, Player*> it : this->players) {
		it.second->updatePlayerExit(player);
	}
}

void Screen::updatePlayerPosition(class Player * player) {
	for(std::pair<int, Player*> it : this->players) {
		it.second->updatePlayer(player);
	}
}

