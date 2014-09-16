#ifndef SCREEN_H

class Screen;
#define SCREEN_H

// ToDO : latter : default spawn position.

#include "place.h"
#include "player.h"
#include "error.h"

#include <string>
#include <vector>
#include <map>
#include <mutex>

class Screen {
	private:
		std::string id;
		std::string name;
		unsigned int width;
		unsigned int height;
		std::vector<class Place> places;
		// std::map<std::string, class Player *> players; // XXX
		std::map<int, class Player *> players;

		bool canLand(class Player * player, unsigned int x, unsigned int y);

	public:
		std::mutex mutex;

		Screen(std::string id, std::string name, unsigned int width, unsigned int height, class Tile* default_tile);
		~Screen();
		std::string getId();
		std::string getName();
		void setName(std::string name);
		unsigned int getWidth();
		unsigned int getHeight();
		class Place * getPlace(unsigned int x, unsigned int y); // May return NULL.
		// class Player * getPlayerById(std::string id); // May return NULL. // XXX
		// class Player * getPlayerByFD(int fd); // May return NULL. // XXX
		class Player * getPlayer(int id_fd); // May return NULL.
		void move(class Player * player, signed int xShift, signed int yShift);
		void enterPlayer(class Player * player); // Add the player to the screen's list. Send the floor to the entering player and call this->updatePlayerPosition(player);
		void exitPlayer(class Player * player); // Remove the player from the screen and broadcast it.
		void event(std::string message); // Broadcast a visible event.
		void updatePlayerPosition(class Player * player); // Broadcast the new position of the player.
};

#endif

