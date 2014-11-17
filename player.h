#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <thread>
#include <map>

#include "aspect.h"
#include "tagged.h"

class Screen;
class Gauge;
class Object;

// TODO : Invisible.
// TODO : Unmovable.

class Player : public Tagged {
	private:
		int fd;
		int id;
		std::string name;
		Aspect aspect;
		class Screen * screen;
		int x;
		int y;
		std::string onDeath;
		std::map<std::string, class Gauge *> gauges;
		std::map<unsigned long int, class Object *> objects;
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

	public:
		Player(int fd, std::string name, Aspect aspect);
		~Player();
		void spawn(class Screen * screen, int x, int y); // Add itself to the screen and start the parsing loop. Do nothing if already running.
		int getId();
		std::string getName();
		void setName(std::string name);
		Aspect getAspect();
		void setAspect(Aspect aspect); // and broadcast it.
		class Screen * getScreen(); // May return NULL.
		unsigned int getX();
		unsigned int getY();
		void setXY(int x, int y); // dont check if canLand(); auto bcast new position.
		void move(int xShift, int yShift); // check if canLand() and setXY() if yes.
		void changeScreen(class Screen * newScreen, int x, int y); // exit this screen, enter the new one.
		std::string getOnDeath();
		void setOnDeath(std::string script);
		class Gauge * getGauge(std::string name); // May return NULL.
		void addGauge(class Gauge * gauge); // Only a new gauge can call it.
		void delGauge(std::string name);
		class Object * getObject(unsigned long int id); // Maux return NULL.
		void addObject(class Object * object);
		void remObject(unsigned long int id); // Don't delete, remove only.

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
		void updateObject(unsigned int x, unsigned int y, Aspect aspect);
		void updateNoObject(unsigned int x, unsigned int y);
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
};

#endif
