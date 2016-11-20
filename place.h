#pragma once

class Zone;

#include "aspect.h"
#include "script.h"
#include "tag.h"

class Place : public Tagged, public Aspected {
public:
	Place() = delete;
	Place(
		const Zone& zone,
		const Aspect& aspect,
		bool walkable = true
	);

	/* Zone handling. */
	const Zone& getZone() const;

	/* Walkable. */
	bool isWalkable() const;
	void setWalkable();
	void setNotWalkable();

	/* When Walked On. */
	const Script& getWhenWalkedOn() const;
	void setWhenWalkedOn(const Script script);
	void resetWhenWalkedOn();

private:
	const Zone& zone;     // The zone in which this is contained.
	bool walkable;        // Can a player walk here?
	Script whenWalkOn {}; // Script triggered when a player walks here.
};
