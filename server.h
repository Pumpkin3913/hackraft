#ifndef SERVER_H
#define SERVER_H

class Screen;
class Tile;
class Luawrapper;
class Player;

#include "error.h"

#include <map>
#include <thread>

#define MAX_SOCKET_QUEUE 8

// TODO : add a mutex to the server to avoid inserting a screen while another thread is iterating through screens.

class Server {
	private:
		int connexion_fd;
		unsigned short port;
		std::map<std::string, class Tile *> tiles;
		std::map<std::string, class Screen *> screens;
		std::map<std::string, std::string *> scripts;
		std::thread * acceptThread;
		class Luawrapper * luawrapper;

		static class Tile defaultTile;

		void acceptLoop();

	public:
		Server();
		~Server();
		void _open(unsigned short port);
		void _close();
		bool isOpen();
		unsigned short getPort();
		void addScreen(std::string id, class Screen * screen);
		class Screen * getScreen(std::string id);
		void delScreen(std::string id);
		void addTile(class Tile * tile);
		class Tile * getTile(std::string id);
		// A Tile must never be remove before the destruction of the server.
		void addScript(std::string id, std::string * filename);
		std::string * getScript(std::string id);
		void delScript(std::string id);
		void exeLua(std::string filename, class Player * player = NULL);
		void exeScript(std::string id, class Player * player = NULL);
};

#endif
