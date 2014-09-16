#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <string>

// TODO : log !

void setVerbose();
void setNoVerbose();
bool isVerbose();

void nonfatal(std::string msg);
void fatal(std::string msg);
void not_enough_memory();
void info(std::string msg);
void verbose_info(std::string msg);

#endif

