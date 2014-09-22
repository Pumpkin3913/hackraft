#ifndef SCREEN_H
#define SCREEN_H

// ToDO : latter : default spawn position.

class Place;
class Player;
class Server;

#include "error.h"

#include <string>
#include <vector>
#include <map>
#include <mutex>

class Screen {
	private:
		class Server * server;
		std::string name;
		unsigned int width;
		unsigned int height;
		std::vector<class Place> places;
		std::map<int, class Player *> players;

	public:
		std::mutex mutex;

		Screen(
			class Server * server,
			std::string name,
			unsigned int width,
			unsigned int height,
			class Tile* default_tile
		);
		~Screen();
		class Server * getServer();
		std::string getName();
		void setName(std::string name);
		unsigned int getWidth();
		unsigned int getHeight();
		class Place * getPlace(int x, int y); // May return NULL.
		class Player * getPlayer(int id_fd); // May return NULL.
		void event(std::string message); // Broadcast a message to all players.

		/* Called by Player only */

		bool canLandPlayer(class Player * player, int x, int y);

		// Add the player to the screen,
		// send him the floor and broadcast its position.
		void enterPlayer(class Player * player);

		// Remove the player from the screen and broadcast it.
		void exitPlayer(class Player * player);

		// Broadcast the new position of the player.
		void updatePlayerPosition(class Player * player);
};

#endif
