#ifndef TILE_H
#define TILE_H

#include <string>

#include "aspect.h"

// TODO : delete aspect, replace id by unsigned int

class Tile {
	private:
		std::string id;
		std::string name;
		Aspect aspect;
		bool _canLand;
	public:
		Tile(std::string id, std::string name, Aspect aspect, bool canLand = true);
		~Tile();
		std::string getId();
		std::string getName();
		void setName(std::string name);
		Aspect getAspect();
		void setAspect(Aspect aspect);
		bool canLand();
		void setCanLand();
		void setCantLand();

		static class Tile defaultTile;

};

#endif
