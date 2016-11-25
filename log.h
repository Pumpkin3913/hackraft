#pragma once

#include <string>

// TODO : log into file.

void setVerbose();
void setNoVerbose();
bool isVerbose();

void info(std::string msg);
void warning(std::string msg);
void fatal(std::string msg);
