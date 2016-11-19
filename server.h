#pragma once

class Zone;
class Tile;
class Luawrapper;
class Player;

#include "error.h"

#include <map>
#include <thread>
#include <mutex>

#define MAX_SOCKET_QUEUE 8

// TODO : Add mutexes around every atomic non-cascading operations.

class Server {
public:
	Server();
	~Server();
	void _open(unsigned short port);
	void _close();
	bool isOpen();
	unsigned short getPort();
	void addZone(std::string id, class Zone * zone); // Automatically done by new Zone().
	class Zone * getZone(std::string id);
	void delZone(std::string id);
	void addTile(class Tile * tile);
	class Tile * getTile(std::string id); // May return nullptr.
	// A Tile must never be removed before the destruction of the server.
	void addPlayer(class Player * player); // Automatically called by new Player()
	class Player * getPlayer(int id); // May return nullptr.
	void delPlayer(int id);
	void remPlayer(int id);
	void addScript(std::string id, std::string * filename);
	std::string * getScript(std::string id);
	void delScript(std::string id);
	void exeScript(std::string id, class Player * player = nullptr, std::string arg = "");
	class Luawrapper * getLua();
	void waitForTerminaison();

private:
	int connexion_fd;
	unsigned short port;
	std::map<std::string, class Tile *> tiles;
	std::map<std::string, class Zone *> zones;
	std::map<int, class Player *> players;
	std::map<std::string, std::string *> scripts;
	std::thread * acceptThread;
	class Luawrapper * luawrapper;
	std::mutex terminaisonLock;

	void acceptLoop();
};
