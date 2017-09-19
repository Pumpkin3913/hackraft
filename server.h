#pragma once

class Zone;
class Luawrapper;
class Character;
class Inventory;

#include "script.h"
#include "uuid.h"
#include "artifact.h"
#include "player.h"

#include <map>
#include <list>
#include <string>

#define MAX_SOCKET_QUEUE 8

class Server {
public:
	Server();
	~Server();

	Server(Server const &) = delete;
	void operator=(Server const &) = delete;

	void _open(unsigned short port, const std::string& spawn_zone, unsigned int spawn_x, unsigned int spawn_y);
	void _close();
	bool isOpen();
	unsigned short getPort();

	void addZone(std::string id, class Zone * zone); // Automatically done by new Zone().
	class Zone * getZone(std::string id);
	void delZone(std::string id);

	void addCharacter(class Character * character);
	class Character * getCharacter(Uuid id); // May return nullptr.
	void delCharacter(Uuid id);
	void remCharacter(Uuid id);

	/* Character actions */
// TODO: Stronger type: trigger.
	void addAction(const Script& script, std::string trigger);
	const Script& getAction(std::string trigger); // May return Script::noValue.
	void delAction(std::string trigger);
	void doAction(std::string trigger, class Character& character, std::string arg = "");

	Uuid newArtifact(Name name);
	void delArtifact(Uuid id);
	class Artifact* getArtifact(Uuid id); // May return nullptr.

	Uuid newInventory(unsigned int size);
	void delInventory(Uuid id);
	class Inventory* getInventory(Uuid id); // May return nullptr.

	/* Timers */
	Uuid addTimer(unsigned int duration, const Script& script);
	void delTimer(Uuid id);
	void triggerTimer(Uuid id);
	unsigned int getTimerRemaining(Uuid id); // 0 is not-found.
	void setTimerRemaining(Uuid id, unsigned int remaining);

	class Luawrapper * getLua();

	void loop();

private:
	int connexion_fd;
	unsigned short port;
	bool stop = false;
	std::map<std::string, class Zone *> zones;
	std::map<Uuid, class Character *> characters;
	std::map<std::string, Script> actions;
	std::map<Uuid, class Artifact *> artifacts;
	std::map<Uuid, class Inventory *> inventories;
	std::list<class Player *> players;

	/* Timers */
	struct Timer { unsigned int remaining; Script script; };
	std::map<Uuid,struct Timer> timers;

	class Luawrapper * luawrapper;

	/* Spawn */
	std::string spawn_zone;
	unsigned int spawn_x;
	unsigned int spawn_y;

	void check_connection();
	void check_action(class Player* player);
	void check_console();
	void check_players();
	void check_timers();
	void step_timers();
};
