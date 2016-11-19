#pragma once

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
public:
	static class Server * server;

	Luawrapper(class Server * server);
	~Luawrapper();
	void executeFile(std::string filename, class Player * player = nullptr, std::string arg = ""); // XXX
	void executeCode(std::string code, class Player * player = nullptr, std::string arg = "");
	void spawnScript(class Player * player);

private:
	lua_State * lua_state;
};
