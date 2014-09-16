#ifndef PLAYER_H

class Player;
#define PLAYER_H

// ToDO : latter : special object slot : map<id,slot>.

#include <string>
#include <thread>

#include "aspect.h"
class Screen;
// class Object;

class Player {
	private:
		int fd;
		// std::string id;
		std::string name;
		std::string description;
		Aspect aspect;
		class Screen* screen;
		unsigned int x;
		unsigned int y;
		// std::list<class Object *> objects;
		// std::map<std::string, class Gauge *> gauges;
		// std::map<std::string, std::string> tags;
		// script onDeath;
		unsigned int movepoints;
		bool visible;
		bool solid;
		bool movable;
		std::thread * loopThread;
		bool stop;

		void send(std::string message);
		std::string receive();
		void _close();
		void loopFunction();

	public:
		// Player(int fd, std::string name, std::string description, Aspect aspect, class Screen * spawnScreen); // XXX
		Player(int fd, std::string name, std::string description, Aspect aspect);
		~Player();
		void spawn(class Screen * screen, unsigned int x, unsigned int y); // Add itself to the screen and start the parsing loop.
		int getFD();
		// std::string getId(); // XXX
		int getId();
		std::string getName();
		void setName(std::string name);
		std::string getDescription();
		void setDescription(std::string description);
		Aspect getAspect();
		void setAspect(Aspect aspect);
		class Screen * getScreen(); // May return NULL.
		void setScreen(class Screen * screen);
		unsigned int getX();
		unsigned int getY();
		void setXY(unsigned int x, unsigned int y);
		// getObject();
		// addObject();
		// remObject();
		// getGauge();
		// addGauge();
		// delGauge();
		// getTag();
		// addTag();
		// delTag();
		unsigned int getMovePoints();
		void setMovePoints(unsigned int points);
		void resetMovePoints();
		bool isVisible();
		void setVisible();
		void setNotVisible();
		bool isSolid();
		void setSolid();
		void setNotSolid();
		bool isMovable();
		void setMovable();
		void setNotMovable();

		void parse(); // Receive one line from the socket and execute it. Return false if socket closed.

		void updatePlayer(class Player * player);
		void updatePlayerExit(class Player * player);
		void updateFloor();
		void updateTile(unsigned int x, unsigned int y, Aspect aspect);
		void updateObject(unsigned int x, unsigned int y, Aspect aspect);
		void updateNoObject(unsigned int x, unsigned int y);
		void message(std::string message);
};

#endif

