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
		std::string name;
		std::string description;
		Aspect aspect;
		class Screen* screen;
		int x;
		int y;
		// std::list<class Object *> objects;
		// std::map<std::string, class Gauge *> gauges;
		// std::map<std::string, std::string> tags;
		// script onDeath;
		/* XXX //
		unsigned int movepoints;
		bool visible;
		bool solid;
		bool movable;
		// XXX */
		std::thread * loopThread;
		bool stop;

		void send(std::string message);
		std::string receive();
		void _close();
		void loopFunction();
		void parse(); // Receive one line from the socket and execute it. Return false if socket closed.

	public:
		Player(int fd, std::string name, std::string description, Aspect aspect);
		~Player();
		void spawn(class Screen * screen, int x, int y); // Add itself to the screen and start the parsing loop. Do nothing if already running.
		int getId(); // return FD.
		std::string getName();
		void setName(std::string name);
		std::string getDescription();
		void setDescription(std::string description);
		Aspect getAspect();
		void setAspect(Aspect aspect); // and broadcast it.
		class Screen * getScreen(); // May return NULL.
		unsigned int getX();
		unsigned int getY();
		void setXY(int x, int y); // dont check if canLand(); auto bcast new position.
		void move(int xShift, int yShift); // check if canLand() and setXY() if yes.
		void changeScreen(class Screen * newScreen, int x, int y); // exit this screen, enter the new one.

		// getObject();
		// addObject();
		// remObject();
		// getGauge();
		// addGauge();
		// delGauge();
		// getTag();
		// addTag();
		// delTag();
		/* XXX //
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
		// XXX */

		/* Send messages to client */
		void message(std::string message);
		void updatePlayer(class Player * player);
		void updatePlayerExit(class Player * player);
		void updateFloor();
		void updateTile(unsigned int x, unsigned int y, Aspect aspect);
		void updateObject(unsigned int x, unsigned int y, Aspect aspect);
		void updateNoObject(unsigned int x, unsigned int y);
};

#endif

