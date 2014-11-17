#include <iostream>
#include <thread>
#ifdef __linux__
#include <unistd.h>
#elif defined _WIN32
#endif

#include "server.h"
#include "luawrapper.h"

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

	server = new Server();
	luaLoopThread = new std::thread(&luaLoop, server);

	server->waitForTerminaison();

	luaLoopThread->detach();
	delete(luaLoopThread);
	info("Exiting...");
}

