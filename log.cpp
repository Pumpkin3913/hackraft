#include "log.h"

#include <iostream>

bool verbose = true;

void setVerbose() {
	verbose = true;
}

void setNoVerbose() {
	verbose = false;
}

bool isVerbose() {
	return(verbose);
}

void info(std::string msg) {
	if(verbose) {
		std::cout << "[INFO] " << msg << std::endl;
	}
}

void warning(std::string msg) {
	std::cout << "[WARNING] " << msg << std::endl;
}

void fatal(std::string msg) {
	std::cout << "[FATAL] " << msg << std::endl;
	exit(1); // XXX ??
}

