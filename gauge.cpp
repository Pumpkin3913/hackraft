#include "gauge.h"
#include "player.h"

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
	if(val > this->max) {
		this->val = this->max;
	} else {
		this->val = val;
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
		this->val = 0;
	} else {
		this->val -= val;
	}
	this->update();
}

unsigned int Gauge::getMax() {
	return(this->max);
}

void Gauge::setMax(unsigned int max) {
	this->max = max;
	if(this->val > max) {
		this->val = max;
	}
	this->update();
}

// Private.

void Gauge::update() {
	this->player->updateGauge(this->name, this->val, this->max);
}

