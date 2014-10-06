#include "luawrapper.h"
#include "server.h"
#include "tile.h"
#include "screen.h"
#include "place.h"
#include "player.h"
#include "error.h"

// TODO : check type and validity (NULL arg -> segfault).

/* Output */

int l_setverbose(lua_State * lua) {
	setVerbose();
	return(0);
}

int l_setnoverbose(lua_State * lua) {
	setNoVerbose();
	return(0);
}

int l_isverbose(lua_State * lua) {
	lua_pushboolean(lua, isVerbose());
	return(0);
}

int l_warning(lua_State * lua) {
	std::string message = lua_tostring(lua, 1);
	warning(message);
	return(0);
}

int l_nonfatal(lua_State * lua) {
	std::string message = lua_tostring(lua, 1);
	nonfatal(message);
	return(0);
}

int l_fatal(lua_State * lua) {
	std::string message = lua_tostring(lua, 1);
	fatal(message);
	return(0);
}

int l_info(lua_State * lua) {
	std::string message = lua_tostring(lua, 1);
	info(message);
	return(0);
}

int l_verboseinfo(lua_State * lua) {
	std::string message = lua_tostring(lua, 1);
	verbose_info(message);
	return(0);
}

/* Server */

int l_server_open(lua_State * lua) {
	class Server * server = (class Server *) lua_touserdata(lua, 1);
	unsigned short int port = lua_tointeger(lua, 2);
	server->_open(port);
	return(0);
}

int l_server_close(lua_State * lua) {
	class Server * server = (class Server *) lua_touserdata(lua, 1);
	server->_close();
	return(0);
}

int l_server_isopen(lua_State * lua) {
	class Server * server = (class Server *) lua_touserdata(lua, 1);
	lua_pushboolean(lua, server->isOpen());
	return(1);
}

int l_server_getport(lua_State * lua) {
	class Server * server = (class Server *) lua_touserdata(lua, 1);
	lua_pushinteger(lua, server->getPort());
	return(1);
}

int l_server_addscreen(lua_State * lua) {
	class Server * server = (class Server *) lua_touserdata(lua, 1);
	std::string id = lua_tostring(lua, 2);
	class Screen * screen = (class Screen *) lua_touserdata(lua, 3);
	server->addScreen(id, screen);
	return(0);
}

int l_server_getscreen(lua_State * lua) {
	class Server * server = (class Server *) lua_touserdata(lua, 1);
	std::string id = lua_tostring(lua, 2);
	class Screen * screen = server->getScreen(id);
	lua_pushlightuserdata(lua, screen);
	return(1);
}

int l_server_delscreen(lua_State * lua) {
	class Server * server = (class Server *) lua_touserdata(lua, 1);
	std::string id = lua_tostring(lua, 2);
	server->delScreen(id);
	return(0);
}

int l_server_addtile(lua_State * lua) {
	class Server * server = (class Server *) lua_touserdata(lua, 1);
	class Tile * tile = (class Tile *) lua_touserdata(lua, 2);
	server->addTile(tile);
	return(0);
}

int l_server_gettile(lua_State * lua) {
	class Server * server = (class Server *) lua_touserdata(lua, 1);
	std::string id = lua_tostring(lua, 2);
	lua_pushlightuserdata(lua, server->getTile(id));
	return(1);
}

int l_server_addscript(lua_State * lua) {
	class Server * server = (class Server *) lua_touserdata(lua, 1);
	std::string id = lua_tostring(lua, 2);
	std::string filename = lua_tostring(lua, 3);
	server->addScript(id, new std::string(filename));
	return(0);
}

int l_server_getscript(lua_State * lua) {
	class Server * server = (class Server *) lua_touserdata(lua, 1);
	std::string id = lua_tostring(lua, 2);
	lua_pushlightuserdata(lua, server->getScript(id));
	return(1);
}

int l_server_delscript(lua_State * lua) {
	class Server * server = (class Server *) lua_touserdata(lua, 1);
	std::string id = lua_tostring(lua, 2);
	server->delScript(id);
	return(0);
}

/* Tile */

int l_new_tile(lua_State * lua) {
	std::string id = lua_tostring(lua, 1);
	std::string name = lua_tostring(lua, 2);
	std::string description = lua_tostring(lua, 3);
	Aspect aspect = lua_tointeger(lua, 4);
	class Tile * tile;
	if(lua_isboolean(lua, 5)) {
		bool canland = lua_toboolean(lua, 5);
		tile = new Tile(id, name, description, aspect, canland);
	} else {
		tile = new Tile(id, name, description, aspect);
	}
	lua_pushlightuserdata(lua, tile);
	return(1);
}

int l_tile_getid(lua_State * lua) {
	class Tile * tile = (class Tile *) lua_touserdata(lua, 1);
	lua_pushstring(lua, tile->getId().c_str());
	return(1);
}

int l_tile_getname(lua_State * lua) {
	class Tile * tile = (class Tile *) lua_touserdata(lua, 1);
	lua_pushstring(lua, tile->getName().c_str());
	return(1);
}

int l_tile_setname(lua_State * lua) {
	class Tile * tile = (class Tile *) lua_touserdata(lua, 1);
	std::string name = lua_tostring(lua, 2);
	tile->setName(name);
	return(0);
}

int l_tile_getdescription(lua_State * lua) {
	class Tile * tile = (class Tile *) lua_touserdata(lua, 1);
	lua_pushstring(lua, tile->getDescription().c_str());
	return(1);
}

int l_tile_setdescription(lua_State * lua) {
	class Tile * tile = (class Tile *) lua_touserdata(lua, 1);
	std::string description = lua_tostring(lua, 2);
	tile->setDescription(description);
	return(0);
}

int l_tile_getaspect(lua_State * lua) {
	class Tile * tile = (class Tile *) lua_touserdata(lua, 1);
	lua_pushinteger(lua, tile->getAspect());
	return(1);
}

int l_tile_setaspect(lua_State * lua) {
	class Tile * tile = (class Tile *) lua_touserdata(lua, 1);
	Aspect aspect = lua_tointeger(lua, 2);
	tile->setAspect(aspect);
	return(0);
}

int l_tile_canland(lua_State * lua) {
	class Tile * tile = (class Tile *) lua_touserdata(lua, 1);
	lua_pushboolean(lua, tile->canLand());
	return(1);
}

int l_tile_setcanland(lua_State * lua) {
	class Tile * tile = (class Tile *) lua_touserdata(lua, 1);
	tile->setCanLand();
	return(0);
}

int l_tile_setcantland(lua_State * lua) {
	class Tile * tile = (class Tile *) lua_touserdata(lua, 1);
	tile->setCantLand();
	return(0);
}

/* Screen */

int l_new_screen(lua_State * lua) {
	std::string name = lua_tostring(lua, 1);
	unsigned int width = lua_tointeger(lua, 2);
	unsigned int height = lua_tointeger(lua, 3);
	class Tile * tile = (class Tile *) lua_touserdata(lua, 4);
	lua_getglobal(lua, "Server");
	class Server * server = (class Server *) lua_touserdata(lua, -1);
	class Screen * screen = new Screen(server, name, width, height, tile);
	lua_pushlightuserdata(lua, screen);
	return(1);
}

int l_screen_getname(lua_State * lua) {
	class Screen * screen = (class Screen *) lua_touserdata(lua, 1);
	lua_pushstring(lua, screen->getName().c_str());
	return(1);
}

int l_screen_setname(lua_State * lua) {
	class Screen * screen = (class Screen *) lua_touserdata(lua, 1);
	std::string name = lua_tostring(lua, 2);
	screen->setName(name);
	return(0);
}

int l_screen_getplace(lua_State * lua) {
	class Screen * screen = (class Screen *) lua_touserdata(lua, 1);
	unsigned int x = lua_tointeger(lua, 2);
	unsigned int y = lua_tointeger(lua, 3);
	class Place * place = screen->getPlace(x, y);
	lua_pushlightuserdata(lua, place);
	return(1);
}

int l_screen_getplayer(lua_State * lua) {
	class Screen * screen = (class Screen *) lua_touserdata(lua, 1);
	int id = lua_tointeger(lua, 2);
	class Player * player = screen->getPlayer(id);
	lua_pushlightuserdata(lua, player);
	return(1);
}

int l_screen_event(lua_State * lua) {
	class Screen * screen = (class Screen *) lua_touserdata(lua, 1);
	std::string message = lua_tostring(lua, 2);
	screen->event(message);
	return(0);
}

int l_screen_updatetile(lua_State * lua) {
	class Screen * screen = (class Screen *) lua_touserdata(lua, 1);
	int x = lua_tointeger(lua, 2);
	int y = lua_tointeger(lua, 3);
	screen->updateTile(x, y);
	return(0);
}

/* Place */

int l_new_place(lua_State * lua) {
	class Tile * tile = (class Tile *) lua_touserdata(lua, 1);
	class Place * place = new Place(tile);
	lua_pushlightuserdata(lua, place);
	return(1);
}

int l_place_gettile(lua_State * lua) {
	class Place * place = (class Place *) lua_touserdata(lua, 1);
	lua_pushlightuserdata(lua, place->getTile());
	return(1);
}

int l_place_settile(lua_State * lua) {
	class Place * place = (class Place *) lua_touserdata(lua, 1);
	class Tile * tile = (class Tile *) lua_touserdata(lua, 2);
	place->setTile(tile);
	return(0);
}

int l_place_getname(lua_State * lua) {
	class Place * place = (class Place *) lua_touserdata(lua, 1);
	lua_pushstring(lua, place->getName().c_str());
	return(1);
}

int l_place_setname(lua_State * lua) {
	class Place * place = (class Place *) lua_touserdata(lua, 1);
	std::string name = lua_tostring(lua, 2);
	place->setName(name);
	return(0);
}

int l_place_resetname(lua_State * lua) {
	class Place * place = (class Place *) lua_touserdata(lua, 1);
	place->resetName();
	return(0);
}

int l_place_getdescription(lua_State * lua) {
	class Place * place = (class Place *) lua_touserdata(lua, 1);
	lua_pushstring(lua, place->getDescription().c_str());
	return(1);
}

int l_place_setdescription(lua_State * lua) {
	class Place * place = (class Place *) lua_touserdata(lua, 1);
	std::string descr = lua_tostring(lua, 2);
	place->setDescription(descr);
	return(0);
}

int l_place_resetdescription(lua_State * lua) {
	class Place * place = (class Place *) lua_touserdata(lua, 1);
	place->resetDescription();
	return(0);
}

int l_place_getaspect(lua_State * lua) {
	class Place * place = (class Place *) lua_touserdata(lua, 1);
	lua_pushinteger(lua, place->getAspect());
	return(1);
}

int l_place_setaspect(lua_State * lua) {
	class Place * place = (class Place *) lua_touserdata(lua, 1);
	Aspect aspect = lua_tointeger(lua, 2);
	place->setAspect(aspect);
	return(0);
}

int l_place_resetaspect(lua_State * lua) {
	class Place * place = (class Place *) lua_touserdata(lua, 1);
	place->resetAspect();
	return(0);
}

int l_place_canland(lua_State * lua) {
	class Place * place = (class Place *) lua_touserdata(lua, 1);
	lua_pushboolean(lua, place->canLand());
	return(1);
}

int l_place_setcanland(lua_State * lua) {
	class Place * place = (class Place *) lua_touserdata(lua, 1);
	place->setCanLand();
	return(0);
}

int l_place_setcantland(lua_State * lua) {
	class Place * place = (class Place *) lua_touserdata(lua, 1);
	place->setCantLand();
	return(0);
}

int l_place_resetcanland(lua_State * lua) {
	class Place * place = (class Place *) lua_touserdata(lua, 1);
	place->resetCanLand();
	return(0);
}

int l_place_getlandon(lua_State * lua) {
	class Place * place = (class Place *) lua_touserdata(lua, 1);
	lua_pushstring(lua, place->getLandon().c_str());
	return(1);
}

int l_place_setlandon(lua_State * lua) {
	class Place * place = (class Place *) lua_touserdata(lua, 1);
	std::string script = lua_tostring(lua, 2);
	place->setLandon(script);
	return(0);
}

int l_place_resetlandon(lua_State * lua) {
	class Place * place = (class Place *) lua_touserdata(lua, 1);
	place->resetLandon();
	return(0);
}

/* Player */

int l_delete_player(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	delete(player);
	return(0);
}

int l_player_spawn(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	class Screen * screen = (class Screen *) lua_touserdata(lua, 2);
	int x = lua_tointeger(lua, 3);
	int y = lua_tointeger(lua, 4);
	player->spawn(screen, x, y);
	return(0);
}

int l_player_getid(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	lua_pushinteger(lua, player->getId());
	return(1);
}

int l_player_getname(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	lua_pushstring(lua, player->getName().c_str());
	return(1);
}

int l_player_setname(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	std::string name = lua_tostring(lua, 2);
	player->setName(name);
	return(0);
}

int l_player_getdescription(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	lua_pushstring(lua, player->getDescription().c_str());
	return(1);
}

int l_player_setdescription(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	std::string descr = lua_tostring(lua, 2);
	player->setDescription(descr);
	return(0);
}

int l_player_getaspect(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	lua_pushinteger(lua, player->getAspect());
	return(1);
}

int l_player_setaspect(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	Aspect aspect = lua_tointeger(lua, 2);
	player->setAspect(aspect);
	return(0);
}

int l_player_getscreen(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	lua_pushlightuserdata(lua, player->getScreen());
	return(1);
}

int l_player_getx(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	lua_pushinteger(lua, player->getX());
	return(1);
}

int l_player_gety(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	lua_pushinteger(lua, player->getY());
	return(1);
}

int l_player_setxy(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	int x = lua_tointeger(lua, 2);
	int y = lua_tointeger(lua, 3);
	player->setXY(x, y);
	return(0);
}

int l_player_move(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	int x = lua_tointeger(lua, 2);
	int y = lua_tointeger(lua, 3);
	player->move(x, y);
	return(0);
}

int l_player_changescreen(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	class Screen * screen = (class Screen *) lua_touserdata(lua, 2);
	int x = lua_tointeger(lua, 3);
	int y = lua_tointeger(lua, 4);
	player->changeScreen(screen, x, y);
	return(0);
}

int l_player_message(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	std::string message = lua_tostring(lua, 2);
	player->message(message);
	return(0);
}

int l_player_follow(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	class Player * target = (class Player *) lua_touserdata(lua, 2);
	player->follow(target);
	return(0);
}

/* Wraper class */

Luawrapper::Luawrapper(class Server * server) :
	lua_state(luaL_newstate()),
	server(server)
{
	luaL_openlibs(this->lua_state);

	lua_pushlightuserdata(this->lua_state, this->server);
	lua_setglobal(this->lua_state, "Server");

	lua_register(this->lua_state, "setverbose", l_setverbose);
	lua_register(this->lua_state, "setnoverbose", l_setnoverbose);
	lua_register(this->lua_state, "isverbose", l_isverbose);
	lua_register(this->lua_state, "warning", l_warning);
	lua_register(this->lua_state, "nonfatal", l_nonfatal);
	lua_register(this->lua_state, "fatal", l_fatal);
	lua_register(this->lua_state, "info", l_info);
	lua_register(this->lua_state, "verboseinfo", l_verboseinfo);

	lua_register(this->lua_state, "server_open", l_server_open);
	lua_register(this->lua_state, "server_close", l_server_close);
	lua_register(this->lua_state, "server_isopen", l_server_isopen);
	lua_register(this->lua_state, "server_getport", l_server_getport);
	lua_register(this->lua_state, "server_addscreen", l_server_addscreen);
	lua_register(this->lua_state, "server_getscreen", l_server_getscreen);
	lua_register(this->lua_state, "server_delscreen", l_server_delscreen);
	lua_register(this->lua_state, "server_addtile", l_server_addtile);
	lua_register(this->lua_state, "server_gettile", l_server_gettile);
	lua_register(this->lua_state, "server_addscript", l_server_addscript);
	lua_register(this->lua_state, "server_getscript", l_server_getscript);
	lua_register(this->lua_state, "server_delscript", l_server_delscript);

	lua_register(this->lua_state, "new_tile", l_new_tile);
	lua_register(this->lua_state, "tile_getid", l_tile_getid);
	lua_register(this->lua_state, "tile_getname", l_tile_getname);
	lua_register(this->lua_state, "tile_setname", l_tile_setname);
	lua_register(this->lua_state, "tile_getdescription", l_tile_getdescription);
	lua_register(this->lua_state, "tile_setdescription", l_tile_setdescription);
	lua_register(this->lua_state, "tile_getaspect", l_tile_getaspect);
	lua_register(this->lua_state, "tile_setaspect", l_tile_setaspect);
	lua_register(this->lua_state, "tile_canland", l_tile_canland);
	lua_register(this->lua_state, "tile_setcanland", l_tile_setcanland);
	lua_register(this->lua_state, "tile_setcantland", l_tile_setcantland);

	lua_register(this->lua_state, "new_screen", l_new_screen);
	lua_register(this->lua_state, "screen_getname", l_screen_getname);
	lua_register(this->lua_state, "screen_setname", l_screen_setname);
	lua_register(this->lua_state, "screen_getplace", l_screen_getplace);
	lua_register(this->lua_state, "screen_getplayer", l_screen_getplayer);
	lua_register(this->lua_state, "screen_event", l_screen_event);
	lua_register(this->lua_state, "screen_updatetile", l_screen_updatetile);

	lua_register(this->lua_state, "new_place", l_new_place);
	lua_register(this->lua_state, "place_gettile", l_place_gettile);
	lua_register(this->lua_state, "place_settile", l_place_settile);
	lua_register(this->lua_state, "place_getname", l_place_getname);
	lua_register(this->lua_state, "place_setname", l_place_setname);
	lua_register(this->lua_state, "place_resetname", l_place_resetname);
	lua_register(this->lua_state, "place_getdescription", l_place_getdescription);
	lua_register(this->lua_state, "place_setdescription", l_place_setdescription);
	lua_register(this->lua_state, "place_resetdescription", l_place_resetdescription);
	lua_register(this->lua_state, "place_getaspect", l_place_getaspect);
	lua_register(this->lua_state, "place_setaspect", l_place_setaspect);
	lua_register(this->lua_state, "place_resetaspect", l_place_resetaspect);
	lua_register(this->lua_state, "place_canland", l_place_canland);
	lua_register(this->lua_state, "place_setcanland", l_place_setcanland);
	lua_register(this->lua_state, "place_setcantland", l_place_setcantland);
	lua_register(this->lua_state, "place_resetcanland", l_place_resetcanland);
	lua_register(this->lua_state, "place_getlandon", l_place_getlandon);
	lua_register(this->lua_state, "place_setlandon", l_place_setlandon);
	lua_register(this->lua_state, "place_resetlandon", l_place_resetlandon);

	lua_register(this->lua_state, "delete_player", l_delete_player);
	lua_register(this->lua_state, "player_spawn", l_player_spawn);
	lua_register(this->lua_state, "player_getid", l_player_getid);
	lua_register(this->lua_state, "player_getname", l_player_getname);
	lua_register(this->lua_state, "player_setname", l_player_setname);
	lua_register(this->lua_state, "player_getdescription", l_player_getdescription);
	lua_register(this->lua_state, "player_setdescription", l_player_setdescription);
	lua_register(this->lua_state, "player_getaspect", l_player_getaspect);
	lua_register(this->lua_state, "player_setaspect", l_player_setaspect);
	lua_register(this->lua_state, "player_getscreen", l_player_getscreen);
	lua_register(this->lua_state, "player_getx", l_player_getx);
	lua_register(this->lua_state, "player_gety", l_player_gety);
	lua_register(this->lua_state, "player_setxy", l_player_setxy);
	lua_register(this->lua_state, "player_move", l_player_move);
	lua_register(this->lua_state, "player_changescreen", l_player_changescreen);
	lua_register(this->lua_state, "player_message", l_player_message);
	lua_register(this->lua_state, "player_follow", l_player_follow);

	this->executeFile(LUA_INIT_SCRIPT);
}

Luawrapper::~Luawrapper() {
	lua_close(this->lua_state);
}

void Luawrapper::executeFile(std::string filename, class Player * player, std::string arg) {
	if(player) {
		lua_pushlightuserdata(this->lua_state, player);
	} else {
		lua_pushnil(this->lua_state);
	}
	lua_setglobal(this->lua_state, "Player");

	if(arg == "") {
		lua_pushnil(this->lua_state);
	} else {
		lua_pushstring(this->lua_state, arg.c_str());
	}
	lua_setglobal(this->lua_state, "Arg");

	luaL_dofile(this->lua_state, filename.c_str());
}

void Luawrapper::executeCode(std::string code) {
	luaL_dostring(this->lua_state, code.c_str());
}

void Luawrapper::spawnScript(class Player * player) {
	this->executeFile(LUA_SPAWN_SCRIPT, player);
}

