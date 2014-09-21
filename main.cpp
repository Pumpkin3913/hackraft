#include <iostream>

#include "server.h"
/*
#include "screen.h"
#include "place.h"
#include "tile.h"
#include "player.h"
#include "aspect.h"
*/
#include "luawrapper.h"
#include "error.h"

int main (int argc, char** argv) {
	class Server * server = new Server();
	std::string input;
	while(getline(std::cin, input)) {
		if(!input.compare("quit")) {
			break;
		}
		server->exeLua(input);
	}

	delete(server);
}

