#include <iostream>
#include <thread>
#include <cstdlib> // srand()
#include <ctime> // time()
#ifdef __linux__
#include <unistd.h>
#elif defined _WIN32
#endif

#include "server.h"
#include "luawrapper.h"
#include "log.h"

void luaLoop(class Server * server) {
	std::string input;
	while(getline(std::cin, input)) {
		server->getLua()->executeCode(input);
	}
}

int main (int argc, char** argv) {
	class Server * server;
	std::thread * luaLoopThread;
	if(argc >= 2) {
		chdir(argv[1]);
	}

	// Seed RNG.
	srand(time(nullptr));

	Uuid a{"123.456"};
	info("TEST: "+a.toString());

	Uuid b{"123456"};
	info("TEST: "+b.toString());

	Uuid c{"plopokio"};
	info("TEST: "+c.toString());

	server = new Server();
	luaLoopThread = new std::thread(&luaLoop, server);

	server->waitForTerminaison();

	luaLoopThread->detach();
	delete(luaLoopThread);
	info("Exiting...");
}

