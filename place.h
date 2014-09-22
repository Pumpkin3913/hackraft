#ifndef PLACE_H
#define PLACE_H

#include <string>

class Tile;

#include "aspect.h"

class Place {
	private:
		class Tile * tile;
		std::string local_name;        // use tile's one if "".
		std::string local_description; // use tile's one if "".
		Aspect local_aspect;           // use tile's one if 0.
		bool useLocalCanLand;
		bool local_canLand;
		// std::unordered_set<class Object *> objects;
		// std::unordered_map<std::string, class Script *> usages;
		// class Script * landon;
	public:
		Place(class Tile * tile);
		~Place();
		class Tile * getTile();
		void setTile(class Tile * tile);
		std::string getName();
		void setName(std::string name);
		void resetName();
		std::string getDescription();
		void setDescription(std::string description);
		void resetDescription();
		Aspect getAspect();
		void setAspect(Aspect aspect);
		void resetAspect();
		bool canLand();
		void setCanLand();
		void setCantLand();
		void resetCanLand();
		// getObject();
		// addObject();
		// remObject();
		// isEmpty();
		// getUsage();
		// addUsage();
		// remUsage();
		// hasUsage();
		// setLandon();
		// resetLandon();
		// hasLandon();
		// troggerLandon();
};

#endif
