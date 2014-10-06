#ifndef LUAWRAPPER_H
#define LUAWRAPPER_H

class Server;
class Player;

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <string>

#define LUA_INIT_SCRIPT "init.lua"
#define LUA_SPAWN_SCRIPT "spawn.lua"

class Luawrapper {
	private:
		lua_State * lua_state;
		class Server * server;
	public:
		Luawrapper(class Server * server);
		~Luawrapper();
		void executeFile(std::string filename, class Player * player = NULL, std::string arg = "");
		void executeCode(std::string code);
		void spawnScript(class Player * player);
};

#endif
