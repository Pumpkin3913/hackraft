#include "gauge.h"
#include "player.h"
#include "screen.h"
#include "server.h"
#include "luawrapper.h"

Gauge::Gauge(
	class Player * player,
	std::string name,
	unsigned int initVal,
	unsigned int max
) :
	player(player),
	name(name),
	val(initVal),
	max(max)
{
	player->addGauge(this);
	this->update();
}

Gauge::~Gauge() {
	this->player->updateNoGauge(this->name);
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

// Private.

void Gauge::update() {
	this->player->updateGauge(this->name, this->val, this->max);
}

void Gauge::exeFull() {
	if(this->onFull != "") {
		class Screen * screen = this->player->getScreen();
		if(screen != NULL) {
			screen->getServer()->getLua()->executeFile(this->onFull, this->player);
		}
	}
}

void Gauge::exeEmpty() {
	if(this->onEmpty != "") {
		class Screen * screen = this->player->getScreen();
		if(screen != NULL) {
			screen->getServer()->getLua()->executeFile(this->onEmpty, this->player);
		}
	}
}

