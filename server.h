#ifndef SERVER_H
#define SERVER_H

class Screen;
class Tile;

#include "error.h"

#include <map>
#include <thread>

#define MAX_SOCKET_QUEUE 8

class Server {
	private:
		int connexion_fd;
		unsigned short port;
		std::map<std::string, class Tile *> tiles;
		std::map<std::string, class Screen *> screens;
		class Screen * spawnScreen;
		std::thread * acceptThread;
		bool stop;

		static class Tile defaultTile;

		void acceptLoop();

	public:
		Server();
		~Server();
		void _open(unsigned short port);
		void _close();
		bool isOpen();
		unsigned short getPort();
		void addScreen(class Screen * screen);
		class Screen * getScreen(std::string id);
		// void rem/delScreen(std::string id); // Don't forget to set spawnScreen to NULL if the spawnScreen is deleted.
		void setSpawnScreen(std::string id);
		bool hasSpawnScreen();
		void addTile(class Tile * tile);
		class Tile * getTile(std::string id);
		// A Tile must never be remove before the destruction of the server.
};

#endif

