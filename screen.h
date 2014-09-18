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
		std::string name;
		unsigned int width;
		unsigned int height;
		std::vector<class Place> places;
		std::map<int, class Player *> players;

	public:
		std::mutex mutex;

		Screen(std::string name, unsigned int width, unsigned int height, class Tile* default_tile);
		~Screen();
		std::string getName();
		void setName(std::string name);
		unsigned int getWidth();
		unsigned int getHeight();
		class Place * getPlace(int x, int y); // May return NULL.
		class Player * getPlayer(int id_fd); // May return NULL.
		void event(std::string message); // Broadcast a message to all players.

		/* Called by Player only */
		bool canLandPlayer(class Player * player, int x, int y);
		void enterPlayer(class Player * player); // Add the player to the screen, send him the floor and broadcast its position.
		void exitPlayer(class Player * player); // Remove the player from the screen and broadcast it.
		void updatePlayerPosition(class Player * player); // Broadcast the new position of the player.
};

#endif

