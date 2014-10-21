#include <iostream>

#include "server.h"
#include "luawrapper.h"

int main (int argc, char** argv) {
	class Server * server = new Server();
	std::string input;
	while(getline(std::cin, input)) {
		if(!input.compare("quit")) {
			break;
		} else {
			server->getLua()->executeCode(input);
		}
	}

	delete(server);
}

