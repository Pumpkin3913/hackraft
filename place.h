#ifndef PLACE_H
#define PLACE_H

#include <string>
#include <list>

class Tile;

#include "aspect.h"

class Place {
	private:
		class Tile * tile;
		std::string local_name; // use tile's one if "".
		Aspect local_aspect;    // use tile's one if 0.
		bool useLocalCanLand;
		bool local_canLand;
		std::list<class Object *> objects;
		std::string * landon;
	public:
		Place(class Tile * tile);
		~Place();
		class Tile * getTile();
		void setTile(class Tile * tile);
		std::string getName();
		void setName(std::string name);
		void resetName();
		Aspect getAspect();
		void setAspect(Aspect aspect);
		void resetAspect();
		bool canLand();
		void setCanLand();
		void setCantLand();
		void resetCanLand();
		std::list<class Object *> * getObjects();
		std::string * getLandOn();
		void setLandOn(std::string script);
		void resetLandOn();
};

#endif
