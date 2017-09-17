#include "gauge.h"
#include "character.h"
#include "zone.h"
#include "server.h"
#include "luawrapper.h"

Gauge::Gauge(
	class Character * character,
	Name name,
	unsigned int initVal,
	unsigned int max,
	const Aspect& aFull,
	const Aspect& aEmpty,
	bool visible
) :
	Named(name),
	character(character),
	val(initVal),
	max(max),
	whenFull(Script::noValue),
	whenEmpty(Script::noValue),
	aFull(aFull),
	aEmpty(aEmpty),
	visible(visible)
{
	character->addGauge(this);
	this->update();
}

Gauge::~Gauge() {
	if(this->visible) {
		this->disapear();
	}
}

unsigned int Gauge::getVal() {
	return(this->val);
}

void Gauge::setVal(unsigned int val) {
	if(val >= this->max) {
		this->val = this->max;
		this->exeFull();
	} else {
		this->val = val;
		if(val == 0) {
			this->exeEmpty();
		}
	}
	this->update();
}

void Gauge::increase(unsigned int val) {
	unsigned int new_val = this->val+val;
	this->setVal(new_val);
	this->update();
}

void Gauge::decrease(unsigned int val) {
	if(val > this->val) {
		this->setVal(0);
	} else {
		this->setVal(this->val - val);
	}
	this->update();
}

unsigned int Gauge::getMax() {
	return(this->max);
}

void Gauge::setMax(unsigned int max) {
	this->max = max;
	if(this->val >= max) {
		this->val = max;
		if(max == 0) {
			this->exeEmpty();
		} else {
			this->exeFull();
		}
	}
	this->update();
}

const Script& Gauge::getWhenFull() {
	return(this->whenFull);
}

void Gauge::setWhenFull(const Script& script) {
	this->whenFull = script;
}

void Gauge::resetWhenFull() {
	this->whenFull = Script::noValue;
}

const Script& Gauge::getWhenEmpty() {
	return(this->whenEmpty);
}

void Gauge::setWhenEmpty(const Script& script) {
	this->whenEmpty = script;
}

void Gauge::resetWhenEmpty() {
	this->whenEmpty = Script::noValue;
}

bool Gauge::isVisible() {
	return(this->visible);
}

void Gauge::setVisible() {
	if(not this->visible) {
		this->visible = true;
		this->update();
	}
}

void Gauge::setNotVisible() {
	if(this->visible) {
		this->visible = false;
		this->disapear();
	}
}

// Private.

void Gauge::update() {
	if(this->visible) {
		this->character->updateGauge(
			this->getName().toString(),
			this->val,
			this->max,
			this->aFull,
			this->aEmpty);
	}
}

void Gauge::disapear() {
	this->character->updateNoGauge(this->getName().toString());
}

void Gauge::exeFull() {
	if(this->whenFull != Script::noValue) {
		class Zone * zone = this->character->getZone();
		if(zone != nullptr) {
			this->whenFull.execute(*(zone->getServer()->getLua()), this->character);
		}
	}
}

void Gauge::exeEmpty() {
	if(this->whenEmpty != Script::noValue) {
		class Zone * zone = this->character->getZone();
		if(zone != nullptr) {
			this->whenEmpty.execute(*(zone->getServer()->getLua()), this->character);
		}
	}
}

