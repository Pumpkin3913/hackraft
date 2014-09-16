#include "error.h"

#ifdef __linux__
#include "string.h" // strerror()
#include "errno.h" // errno
#endif

bool verbose;


void setVerbose() {
	verbose = true;
}

void setNoVerbose() {
	verbose = false;
}

bool isVerbose() {
	return(verbose);
}


void nonfatal(std::string msg) {
#ifdef __linux__
	std::cerr << "[ERROR] " << msg << " : " << strerror(errno) << std::endl;
#else
	std::cerr << "[ERROR] " << msg << std::endl;
#endif
}

void fatal(std::string msg) {
	nonfatal(msg);
	exit(1);
}

void not_enough_memory() {
	// nonfatal("Not enough memory");
	fatal("Not enough memory");
}

void info(std::string msg) {
	std::cout << "[INFO] " << msg << std::endl;
}

void verbose_info(std::string msg) {
	if(verbose) {
		info(msg);
	}
}

