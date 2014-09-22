#ifndef GAUGE_H
#define GAUGE_H

#include <string>

// #include "script.h"

class Gauge {
	private:
		std::string id;
		std::string name;
		unsigned int val;
		unsigned int max;
		bool visible;
		// script * onFull;
		// script * onEmpty;
	public:
		Gauge(unsigned int initVal, unsigned int max);
		Gauge(unsigned int initVal, unsigned int max, visible);
		~Gauge();
		std::string getId();
		std::string getName();
		void setName(std::string name);
		unsigned int getVal();
		void setVal(unsigned int val);
		void increase(unsigned int val);
		void decrease(unsigned int val);
		unsigned int getMax();
		void setMax(unsigned int max);
		bool isVisible();
		void setVisible();
		void setNotVisible();
		// set/get onfull/onempty();
};

#endif
