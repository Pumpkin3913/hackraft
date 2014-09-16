#include "screen.h"

// PRIVATE

bool Screen::canLand(class Player * player, unsigned int x, unsigned int y) {
	class Place * dst = this->getPlace(x,y);
	if(dst != NULL) {
		return(dst->canLand());
	} else {
		return(false);
	}
}

// PUBLIC

Screen::Screen(std::string id, std::string name, unsigned int width, unsigned int height, class Tile* defaultTile) :
	id(id),
	name(name),
	width(width),
	height(height)
{
	class Place defaultPlace(defaultTile);
	this->places = std::vector<class Place>(width * height, defaultPlace);
}

Screen::~Screen() {
	// For now, players in a screen are deleted with it.
	// for(std::pair<const std::basic_string<char>, Player*> it : this->players) { // XXX
	for(std::pair<int, Player*> it : this->players) {
		delete(it.second);
	}
}

std::string Screen::getId() {
	return(this->id);
}

std::string Screen::getName() {
	return(this->name);
}

void Screen::setName(std::string name) {
	this->name = name;
}

unsigned int Screen::getWidth() {
	return(this->width);
}

unsigned int Screen::getHeight() {
	return(this->height);
}

class Place * Screen::getPlace(unsigned int x, unsigned int y) {
	if(x < this->width && y < this->height) {
		return(&(this->places[y*this->width+x]));
	} else {
		return(NULL);
	}
}

/* XXX //
class Player * Screen::getPlayerById(std::string id) {
	return(this->players[id]);
}

class Player * Screen::getPlayerByFD(int fd) {
	class Player * toReturn = NULL;
	for(std::pair<const std::basic_string<char>, Player*> it : this->players) {
		if(it.second->getFD() == fd) {
			toReturn = it.second;
		}
	}
	return(toReturn);
}
// XXX */

class Player * Screen::getPlayer(int id_fd) {
	return(this->players[id_fd]);
}

void Screen::move(class Player * player, signed int xShift, signed int yShift) {
	unsigned int x = player->getX() + xShift;
	unsigned int y = player->getY() + yShift;
	if(this->canLand(player, x, y)) {
		player->setXY(x, y);
		this->updatePlayerPosition(player);
	}
}

void Screen::enterPlayer(class Player * player) {
	this->players[player->getId()] = player;
	player->setScreen(this);
	// player->updateFloor(this->width, this->height, this->places);
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
	// for(std::pair<const std::basic_string<char>, Player*> it : this->players) { // XXX
	for(std::pair<int, Player*> it : this->players) {
		it.second->updatePlayerExit(player);
	}
}

// Broadcast a visible event.
void Screen::event(std::string message) {
	for(std::pair<int, Player*> it : this->players) {
		it.second->message(message);
	}
}

void Screen::updatePlayerPosition(class Player * player) {
	for(std::pair<int, Player*> it : this->players) {
		it.second->updatePlayer(player);
	}
}

