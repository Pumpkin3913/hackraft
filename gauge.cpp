#include "gauge.h"
#include "player.h"
#include "zone.h"
#include "server.h"
#include "luawrapper.h"

Gauge::Gauge(
	class Player * player,
	std::string name,
	unsigned int initVal,
	unsigned int max,
	Aspect aFull,
	Aspect aEmpty,
	bool visible
) :
	player(player),
	name(name),
	val(initVal),
	max(max),
	onFull(""),
	onEmpty(""),
	aFull(aFull),
	aEmpty(aEmpty),
	visible(visible)
{
	player->addGauge(this);
	this->update();
}

Gauge::~Gauge() {
	if(this->visible) {
		this->disapear();
	}
}

std::string Gauge::getName() {
	return(this->name);
}

void Gauge::setName(std::string name) {
	this->name = name;
	this->update();
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

std::string Gauge::getOnFull() {
	return(this->onFull);
}

void Gauge::setOnFull(std::string script) {
	this->onFull = script;
}

void Gauge::resetOnFull() {
	this->onFull = "";
}

std::string Gauge::getOnEmpty() {
	return(this->onEmpty);
}

void Gauge::setOnEmpty(std::string script) {
	this->onEmpty = script;
}

void Gauge::resetOnEmpty() {
	this->onEmpty = "";
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
		this->player->updateGauge(
			this->name,
			this->val,
			this->max,
			this->aFull,
			this->aEmpty);
	}
}

void Gauge::disapear() {
	this->player->updateNoGauge(this->name);
}

void Gauge::exeFull() {
	if(this->onFull != "") {
		class Zone * zone = this->player->getZone();
		if(zone != nullptr) {
			zone->getServer()->getLua()->executeFile(this->onFull, this->player);
		}
	}
}

void Gauge::exeEmpty() {
	if(this->onEmpty != "") {
		class Zone * zone = this->player->getZone();
		if(zone != nullptr) {
			zone->getServer()->getLua()->executeFile(this->onEmpty, this->player);
		}
	}
}

