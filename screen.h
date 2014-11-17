#ifndef SCREEN_H
#define SCREEN_H

class Place;
class Server;
class Tile;
class Place;
class Player;
class Object;

#include "error.h"
#include "aspect.h"

#include <string>
#include <vector>
#include <list>

class Screen {
	private:
		class Server * server;
		std::string id;
		std::string name;
		unsigned int width;
		unsigned int height;
		std::vector<class Place> places;
		std::list<int> players;

		class Player * getPlayer(int id); // Auto remove if invalid.
		class Place * getPlace(int x, int y);
		void updateObject(int x, int y);
		void updateTile(int x, int y);

	public:
		Screen(
			class Server * server,
			std::string id,
			std::string name,
			unsigned int width,
			unsigned int height,
			class Tile * baseTile
		);
		~Screen();
		class Server * getServer();
		std::string getId();
		std::string getName();
		void setName(std::string name);
		unsigned int getWidth();
		unsigned int getHeight();
		bool isPlaceValid(unsigned int x, unsigned int y);
		class Tile * getTile(int x, int y); // May return NULL.
		void setTile(int x, int y, class Tile * tile); // And broadcast it.
		class Object * getTopObject(int x, int y); // May return NULL.
		class Object * getObject(int x, int y, unsigned long int id); // Mau return NULL.
		const std::list<class Object *> * getObjectList(int x, int y); // Only called by Player.
		void addObject(int x, int y, class Object * object);
		void remObject(int x, int y, unsigned long int id); // Don't delete; remove only.
		std::string * getLandOn(int x, int y); // May return NULL.
		void setLandOn(int x, int y, std::string script);
		void resetLandOn(int x, int y);
		void event(std::string message); // Broadcast a message to all players.

		/* Called by Player only */

		bool canLandPlayer(class Player * player, int x, int y);

		// Add the player to the screen,
		// send him the floor and broadcast its position.
		void enterPlayer(class Player * player, int x, int y);

		// Remove the player from the screen and broadcast it.
		void exitPlayer(class Player * player);

		// Broadcast the new position and aspect of the player.
		void updatePlayer(class Player * player);
};

#endif
