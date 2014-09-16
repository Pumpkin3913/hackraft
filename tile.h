#ifndef TILE_H

class Tile;
#define TILE_H

#include <string>

#include "aspect.h"

class Tile {
	private:
		std::string id;
		std::string name;
		std::string description;
		Aspect aspect;
		bool _canLand;
	public:
		Tile(std::string id, std::string name, std::string description, Aspect aspect);
		Tile(std::string id, std::string name, std::string description, Aspect aspect, bool canLand);
		~Tile();
		std::string getId();
		std::string getName();
		void setName(std::string name);
		std::string getDescription();
		void setDescription(std::string description);
		Aspect getAspect();
		void setAspect(Aspect aspect);
		bool canLand();
		void setCanLand();
		void setCantLand();
};

#endif

