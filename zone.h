#pragma once

class Server;
class Place;
class Character;

#include "aspect.h"
#include "name.h"
#include "uuid.h"

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

	void event(std::string message); // Broadcast a message to all characters.

	/* Called by Character only */

	bool canLandCharacter(class Character * character, int x, int y);

	// Add the character to the zone,
	// send him the floor and broadcast its position.
	void enterCharacter(class Character * character, int x, int y);

	// Remove the character from the zone and broadcast it.
	void exitCharacter(class Character * character);

	// Broadcast the new position and aspect of the character.
	void updateCharacter(class Character * character);

private:
	class Server * server;
	std::string id;
	unsigned int width;
	unsigned int height;
	std::vector<class Place> places;
	std::list<Uuid> characters;

	class Character * getCharacter(Uuid id); // Auto remove if invalid.
};
