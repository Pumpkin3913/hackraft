#ifndef LUAWRAPPER_H
#define LUAWRAPPER_H

class Server;

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <string>

class Luawrapper {
	private:
		lua_State * lua_state;
		class Server * server;
	public:
		Luawrapper(class Server * server);
		~Luawrapper();
		void exeLua(std::string filename);
};

#endif

