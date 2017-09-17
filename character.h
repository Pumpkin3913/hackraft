#pragma once

#include <string>
#include <map>

#include "aspect.h"
#include "name.h"
#include "tag.h"
#include "script.h"
#include "uuid.h"

class Player;
class Zone;
class Gauge;

// TODO : Invisible.
// TODO : Unmovable.

class Character : public Aspected, public Named, public Tagged {
public:
	Character(Uuid id, Name name, const Aspect& aspect);
	~Character();
	void spawn(class Zone * zone, int x, int y); // Add itself to the zone and start the parsing loop. Do nothing if already running.
	Uuid getId();
	class Zone * getZone(); // May return nullptr.
	unsigned int getX();
	unsigned int getY();
	void setXY(int x, int y); // dont check if canLand(); auto bcast new position.
	void move(int xShift, int yShift); // check if canLand() and setXY() if yes.
	void changeZone(class Zone * newZone, int x, int y); // exit this zone, enter the new one.
	void set_player(class Player * player);

	/* Scripts */
	const Script& getWhenDeath();
	void setWhenDeath(const Script& script);

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
	void updateCharacter(class Character * character);
	void updateCharacterExit(class Character * character);
	void updateFloor();
	void updateFloor(unsigned int x, unsigned int y, const Aspect& aspect);
	void updateGauge(
		std::string name,
		unsigned int val,
		unsigned int max,
		const Aspect& full,
		const Aspect& empty);
	void updateNoGauge(std::string name);
/* XXX //
	void updateInventory(unsigned long int id, Aspect aspect);
	void updateNoInventory(unsigned long int id);
	void addPickupList(unsigned long int id, Aspect aspect);
	void remPickupList(unsigned long int id);
// XXX */
	void follow(class Character * character);
	void hint(Aspect aspect, std::string hint);

private:
	class Player* player; // May be nullptr.

	Uuid id;
	class Zone * zone;
	int x;
	int y;
	Script whenDeath;
	std::map<std::string, class Gauge *> gauges;
	bool ghost;
/*
	unsigned int movepoints;
	bool visible;
	bool movable;
*/
};
