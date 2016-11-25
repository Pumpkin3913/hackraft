#pragma once

class Server;
class Place;
class Player;

#include "aspect.h"
#include "name.h"

#include <string>
#include <vector>
#include <list>

class Zone : public Named {
public:
	Zone(
		class Server * server,
		std::string id,
		const Name& name,
		unsigned int width,
		unsigned int height,
		const Aspect& base_aspect
	);
	~Zone();

	class Server * getServer();
	std::string getId();

	unsigned int getWidth();
	unsigned int getHeight();
	bool isPlaceValid(int x, int y);
	class Place * getPlace(int x, int y);
	void updatePlaceAspect(int x, int y);

	void event(std::string message); // Broadcast a message to all players.

	/* Called by Player only */

	bool canLandPlayer(class Player * player, int x, int y);

	// Add the player to the zone,
	// send him the floor and broadcast its position.
	void enterPlayer(class Player * player, int x, int y);

	// Remove the player from the zone and broadcast it.
	void exitPlayer(class Player * player);

	// Broadcast the new position and aspect of the player.
	void updatePlayer(class Player * player);

private:
	class Server * server;
	std::string id;
	unsigned int width;
	unsigned int height;
	std::vector<class Place> places;
	std::list<int> players;

	class Player * getPlayer(int id); // Auto remove if invalid.
};
