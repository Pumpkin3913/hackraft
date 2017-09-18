#include <iostream>
#include <thread>
#include <cstdlib> // srand()
#include <ctime> // time()
#include <unistd.h>

#include <signal.h>

#include "server.h"
#include "luawrapper.h"
#include "log.h"

int main (int argc, char** argv) {
	signal(SIGPIPE, SIG_IGN); // Ignore broken pipes.

	if(argc >= 2) {
		chdir(argv[1]);
	}

	// Seed RNG.
	srand(time(nullptr));

	class Server * server = new Server();
	server->loop();
	info("Exiting...");
	delete(server);
}

