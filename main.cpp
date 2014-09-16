#include "server.h"
#include "screen.h"
#include "place.h"
#include "tile.h"
#include "player.h"
#include "aspect.h"
#include "error.h"

#include <iostream>

int main (int argc, char** argv) {
	setVerbose();
	class Server * server = new Server();
	server->addTile(new Tile("floor", "Floor", "An old paved floor.", (Aspect) '.'));
	server->addTile(new Tile("portal", "Portal", "A magic portal leading elsewere.", (Aspect) '^'));
	server->addScreen(new Screen("entrance", "Entrance", 11, 11, server->getTile("floor")));
	server->getScreen("entrance")->getPlace(5,5)->setTile(server->getTile("portal"));
	server->_open(31337);
	std::cout << "Press ENTER to exit." << std::endl;
	std::cin.get();
	delete(server);
}

