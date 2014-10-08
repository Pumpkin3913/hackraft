#ifndef GAUGE_H
#define GAUGE_H

#include <string>

class Player;

class Gauge {
	private:
		class Player * player;
		std::string name;
		unsigned int val;
		unsigned int max;
		// bool visible; // TODO : latter.
		// script * onFull;
		// script * onEmpty;
		// TODO : Aspect full; Aspect empty;
		void update();
	public:
		Gauge(Player * player, std::string name, unsigned int initVal, unsigned int max);
		~Gauge();
		std::string getName();
		void setName(std::string name);
		unsigned int getVal();
		void setVal(unsigned int val);
		void increase(unsigned int val);
		void decrease(unsigned int val);
		unsigned int getMax();
		void setMax(unsigned int max);
		/* TODO : latter.
		bool isVisible();
		void setVisible();
		void setNotVisible();
		*/
		// set/get onfull/onempty();
};

#endif
