#pragma once

#include <string>
#include <thread>
#include <map>

#include "aspect.h"
#include "name.h"
#include "tag.h"

class Zone;
class Gauge;

// TODO : Invisible.
// TODO : Unmovable.

class Player : public Named, public Tagged {
public:
	Player(int fd, Name name, Aspect aspect);
	~Player();
	void spawn(class Zone * zone, int x, int y); // Add itself to the zone and start the parsing loop. Do nothing if already running.
	int getId();
	Aspect getAspect();
	void setAspect(Aspect aspect); // and broadcast it.
	class Zone * getZone(); // May return nullptr.
	unsigned int getX();
	unsigned int getY();
	void setXY(int x, int y); // dont check if canLand(); auto bcast new position.
	void move(int xShift, int yShift); // check if canLand() and setXY() if yes.
	void changeZone(class Zone * newZone, int x, int y); // exit this zone, enter the new one.
	std::string getOnDeath();
	void setOnDeath(std::string script);
	class Gauge * getGauge(const Name& name); // May return nullptr.
	void addGauge(class Gauge * gauge); // Only a new gauge can call it.
	void delGauge(const Name& name);

	bool isGhost();
	void setGhost();
	void setNotGhost();
/*
	unsigned int getMovePoints();
	void setMovePoints(unsigned int points);
	void resetMovePoints();
	bool isVisible();
	void setVisible();
	void setNotVisible();
	bool isMovable();
	void setMovable();
	void setNotMovable();
*/

	/* Send messages to client */
	void message(std::string message);
	void updatePlayer(class Player * player);
	void updatePlayerExit(class Player * player);
	void updateFloor();
	void updateTile(unsigned int x, unsigned int y, Aspect aspect);
	void updateGauge(
		std::string name,
		unsigned int val,
		unsigned int max,
		Aspect full,
		Aspect empty);
	void updateNoGauge(std::string name);
	void updateInventory(unsigned long int id, Aspect aspect);
	void updateNoInventory(unsigned long int id);
	void addPickupList(unsigned long int id, Aspect aspect);
	void remPickupList(unsigned long int id);
	void follow(class Player * player);

private:
	int fd;
	int id;
	Aspect aspect;
	class Zone * zone;
	int x;
	int y;
	std::string onDeath;
	std::map<std::string, class Gauge *> gauges;
	bool ghost;
/*
	unsigned int movepoints;
	bool visible;
	bool movable;
*/
	std::thread * loopThread;
	bool stop;

	void send(std::string message);
	std::string receive();
	void _close();
	void loopFunction();
	void parse(); // Receive one line from the socket and execute it. Return false if socket closed.
};
