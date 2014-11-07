#include "luawrapper.h"
#include "server.h"
#include "tile.h"
#include "screen.h"
#include "player.h"
#include "gauge.h"
#include "object.h"
#include "error.h"

#include <cstdlib> // srand(), rand()
#include <ctime> // time()

void lua_arg_error(std::string msg) {
	warning("Lua must call '"+msg+"'.");
}

int l_c_rand(lua_State * lua) {
	if(not lua_isnumber(lua, 1)) {
		lua_arg_error("c_rand(max)");
	} else {
		static bool done = false;
		if(not done) {
			srand(time(NULL));
			done = true;
		}
		int max = lua_tointeger(lua, 1);
		int x = rand()%max+1;
		lua_pushinteger(lua, x);
		return(1);
	}
	return(0);
}

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
	return(1);
}

int l_warning(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("warning(message)");
	} else {
		std::string message = lua_tostring(lua, 1);
		warning(message);
	}
	return(0);
}

int l_nonfatal(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("nonfatal(message)");
	} else {
		std::string message = lua_tostring(lua, 1);
		nonfatal(message);
	}
	return(0);
}

int l_fatal(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("fatal(message)");
	} else {
		std::string message = lua_tostring(lua, 1);
		fatal(message);
	}
	return(0);
}

int l_info(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("info(message)");
	} else {
		std::string message = lua_tostring(lua, 1);
		info(message);
	}
	return(0);
}

int l_verboseinfo(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("verboseinfo(message)");
	} else {
		std::string message = lua_tostring(lua, 1);
		verbose_info(message);
	}
	return(0);
}

/* Server */

int l_delete_server(lua_State * lua) {
	class Server * server = (class Server *) lua_touserdata(lua, 1);
	if(server == NULL) {
		lua_arg_error("delete_server(server)");
	} else {
		delete(server);
	}
	return(0);
}


int l_server_open(lua_State * lua) {
	class Server * server = (class Server *) lua_touserdata(lua, 1);
	if(server == NULL or not lua_isnumber(lua, 2)) {
		lua_arg_error("server_open(server, port)");
	} else {
		unsigned short int port = lua_tointeger(lua, 2);
		server->_open(port);
	}
	return(0);
}

int l_server_close(lua_State * lua) {
	class Server * server = (class Server *) lua_touserdata(lua, 1);
	if(server == NULL) {
		lua_arg_error("server_close(server)");
	} else {
		server->_close();
	}
	return(0);
}

int l_server_isopen(lua_State * lua) {
	class Server * server = (class Server *) lua_touserdata(lua, 1);
	if(server == NULL) {
		lua_arg_error("server_isopen(server)");
		return(0);
	} else {
		lua_pushboolean(lua, server->isOpen());
		return(1);
	}
}

int l_server_getport(lua_State * lua) {
	class Server * server = (class Server *) lua_touserdata(lua, 1);
	if(server == NULL) {
		lua_arg_error("server_getport(server)");
		return(0);
	} else {
		lua_pushinteger(lua, server->getPort());
		return(1);
	}
}

int l_server_addscreen(lua_State * lua) {
	class Server * server = (class Server *) lua_touserdata(lua, 1);
	class Screen * screen = (class Screen *) lua_touserdata(lua, 3);
	if(server == NULL or not lua_isstring(lua, 2) or screen == NULL) {
		lua_arg_error("server_addscreen(server, id, screen)");
	} else {
		std::string id = lua_tostring(lua, 2);
		server->addScreen(id, screen);
	}
	return(0);
}

int l_server_getscreen(lua_State * lua) {
	class Server * server = (class Server *) lua_touserdata(lua, 1);
	if(server == NULL or not lua_isstring(lua, 2)) {
		lua_arg_error("server_getscreen(server, id)");
	} else {
		std::string id = lua_tostring(lua, 2);
		class Screen * screen = server->getScreen(id);
		lua_pushlightuserdata(lua, screen);
		return(1);
	}
	return(0);
}

int l_server_delscreen(lua_State * lua) {
	class Server * server = (class Server *) lua_touserdata(lua, 1);
	if(server == NULL or not lua_isstring(lua, 2)) {
		lua_arg_error("server_delscreen(server, id)");
	} else {
		std::string id = lua_tostring(lua, 2);
		server->delScreen(id);
	}
	return(0);
}

int l_server_addtile(lua_State * lua) {
	class Server * server = (class Server *) lua_touserdata(lua, 1);
	class Tile * tile = (class Tile *) lua_touserdata(lua, 2);
	if(server == NULL or tile == NULL) {
		lua_arg_error("server_addtile(server, tile)");
	} else {
		server->addTile(tile);
	}
	return(0);
}

int l_server_gettile(lua_State * lua) {
	class Server * server = (class Server *) lua_touserdata(lua, 1);
	if(server == NULL or not lua_isstring(lua, 2)) {
		lua_arg_error("server_gettile(server, id)");
	} else {
		std::string id = lua_tostring(lua, 2);
		lua_pushlightuserdata(lua, server->getTile(id));
		return(1);
	}
	return(0);
}

int l_server_getplayer(lua_State * lua) {
	class Server * server = (class Server *) lua_touserdata(lua, 1);
	if(server == NULL or not lua_isnumber(lua, 2)) {
		lua_arg_error("server_getplayer(server, id)");
	} else {
		int id = lua_tointeger(lua, 2);
		lua_pushlightuserdata(lua, server->getPlayer(id));
		return(1);
	}
	return(0);
}

int l_server_addscript(lua_State * lua) {
	class Server * server = (class Server *) lua_touserdata(lua, 1);
	if(server == NULL or not lua_isstring(lua, 2) or not lua_isstring(lua, 3)) {
		lua_arg_error("server_addscript(server, id, filename)");
	} else {
		std::string id = lua_tostring(lua, 2);
		std::string filename = lua_tostring(lua, 3);
		server->addScript(id, new std::string(filename));
	}
	return(0);
}

int l_server_getscript(lua_State * lua) {
	class Server * server = (class Server *) lua_touserdata(lua, 1);
	if(server == NULL or not lua_isstring(lua, 2)) {
		lua_arg_error("server_getscript(server, id)");
	} else {
		std::string id = lua_tostring(lua, 2);
		lua_pushlightuserdata(lua, server->getScript(id));
		return(1);
	}
	return(0);
}

int l_server_delscript(lua_State * lua) {
	class Server * server = (class Server *) lua_touserdata(lua, 1);
	if(server == NULL or not lua_isstring(lua, 2)) {
		lua_arg_error("server_delscript(server, id)");
	} else {
		std::string id = lua_tostring(lua, 2);
		server->delScript(id);
	}
	return(0);
}

/* Tile */

int l_new_tile(lua_State * lua) {
	if(not lua_isstring(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isnumber(lua, 3)) {
		lua_arg_error("new_tile(id, name, aspect [, passable])");
	} else {
		std::string id = lua_tostring(lua, 1);
		std::string name = lua_tostring(lua, 2);
		Aspect aspect = lua_tointeger(lua, 3);
		class Tile * tile;
		if(lua_isboolean(lua, 4)) {
			bool canland = lua_toboolean(lua, 4);
			tile = new Tile(id, name, aspect, canland);
		} else {
			tile = new Tile(id, name, aspect);
		}
		lua_pushlightuserdata(lua, tile);
		return(1);
	}
	return(0);
}

int l_tile_getid(lua_State * lua) {
	class Tile * tile = (class Tile *) lua_touserdata(lua, 1);
	if(tile == NULL) {
		lua_arg_error("tile_getid(tile)");
	} else {
		lua_pushstring(lua, tile->getId().c_str());
		return(1);
	}
	return(0);
}

int l_tile_getname(lua_State * lua) {
	class Tile * tile = (class Tile *) lua_touserdata(lua, 1);
	if(tile == NULL) {
		lua_arg_error("tile_getname(tile)");
	} else {
		lua_pushstring(lua, tile->getName().c_str());
		return(1);
	}
	return(0);
}

int l_tile_setname(lua_State * lua) {
	class Tile * tile = (class Tile *) lua_touserdata(lua, 1);
	if(tile == NULL or not lua_isstring(lua, 2)) {
		lua_arg_error("tile_setname(tile, name)");
	} else {
		std::string name = lua_tostring(lua, 2);
		tile->setName(name);
	}
	return(0);
}

int l_tile_getaspect(lua_State * lua) {
	class Tile * tile = (class Tile *) lua_touserdata(lua, 1);
	if(tile == NULL) {
		lua_arg_error("tile_getaspect(tile)");
	} else {
		lua_pushinteger(lua, tile->getAspect());
		return(1);
	}
	return(0);
}

int l_tile_setaspect(lua_State * lua) {
	class Tile * tile = (class Tile *) lua_touserdata(lua, 1);
	if(tile == NULL or not lua_isnumber(lua, 2)) {
		lua_arg_error("tile_setaspect(tile, aspect)");
	} else {
		Aspect aspect = lua_tointeger(lua, 2);
		tile->setAspect(aspect);
	}
	return(0);
}

int l_tile_canland(lua_State * lua) {
	class Tile * tile = (class Tile *) lua_touserdata(lua, 1);
	if(tile == NULL) {
		lua_arg_error("tile_canland(tile)");
	} else {
		lua_pushboolean(lua, tile->canLand());
		return(1);
	}
	return(0);
}

int l_tile_setcanland(lua_State * lua) {
	class Tile * tile = (class Tile *) lua_touserdata(lua, 1);
	if(tile == NULL) {
		lua_arg_error("tile_setcanland(tile)");
	} else {
		tile->setCanLand();
	}
	return(0);
}

int l_tile_setcantland(lua_State * lua) {
	class Tile * tile = (class Tile *) lua_touserdata(lua, 1);
	if(tile == NULL) {
		lua_arg_error("tile_setcantland(tile)");
	} else {
		tile->setCantLand();
	}
	return(0);
}

/* Screen */

int l_new_screen(lua_State * lua) {
	class Tile * tile = (class Tile *) lua_touserdata(lua, 4);
	if(not lua_isstring(lua, 1)
			or not lua_isnumber(lua, 2)
			or not lua_isnumber(lua, 3)
			or tile == NULL) {
		lua_arg_error("new_screen(server, name, width, height, tile)");
	} else {
		std::string name = lua_tostring(lua, 1);
		unsigned int width = lua_tointeger(lua, 2);
		unsigned int height = lua_tointeger(lua, 3);
		lua_getglobal(lua, "Server");
		class Server * server = (class Server *) lua_touserdata(lua, -1);
		class Screen * screen = new Screen(server, name, width, height, tile);
		lua_pushlightuserdata(lua, screen);
		return(1);
	}
	return(0);
}

int l_screen_getname(lua_State * lua) {
	class Screen * screen = (class Screen *) lua_touserdata(lua, 1);
	if(screen == NULL) {
		lua_arg_error("screen_getname(screen)");
	} else {
		lua_pushstring(lua, screen->getName().c_str());
		return(1);
	}
	return(0);
}

int l_screen_setname(lua_State * lua) {
	class Screen * screen = (class Screen *) lua_touserdata(lua, 1);
	if(screen == NULL or not lua_isstring(lua, 2)) {
		lua_arg_error("screen_setname(screen, name)");
	} else {
		std::string name = lua_tostring(lua, 2);
		screen->setName(name);
	}
	return(0);
}

int l_screen_getwidth(lua_State * lua) {
	class Screen * screen = (class Screen *) lua_touserdata(lua, 1);
	if(screen == NULL) {
		lua_arg_error("screen_getwidth(screen)");
	} else {
		lua_pushinteger(lua, screen->getWidth());
		return(1);
	}
	return(0);
}

int l_screen_getheight(lua_State * lua) {
	class Screen * screen = (class Screen *) lua_touserdata(lua, 1);
	if(screen == NULL) {
		lua_arg_error("screen_getheight(screen)");
	} else {
		lua_pushinteger(lua, screen->getHeight());
		return(1);
	}
	return(0);
}

int l_screen_gettile(lua_State * lua) {
	class Screen * screen = (class Screen *) lua_touserdata(lua, 1);
	if(screen == NULL or not lua_isnumber(lua, 2) or not lua_isnumber(lua, 3)) {
		lua_arg_error("screen_gettile(screen, x, y)");
	} else {
		unsigned int x = lua_tointeger(lua, 2);
		unsigned int y = lua_tointeger(lua, 3);
		class Tile * tile = screen->getTile(x, y);
		lua_pushlightuserdata(lua, tile);
		return(1);
	}
	return(0);
}

int l_screen_settile(lua_State * lua) {
	class Screen * screen = (class Screen *) lua_touserdata(lua, 1);
	class Tile * tile = (class Tile *) lua_touserdata(lua, 4);
	if(screen == NULL or not lua_isnumber(lua, 2) or not lua_isnumber(lua, 3) or tile == NULL) {
		lua_arg_error("screen_settile(screen, x, y, tile)");
	} else {
		unsigned int x = lua_tointeger(lua, 2);
		unsigned int y = lua_tointeger(lua, 3);
		screen->setTile(x, y, tile);
	}
	return(0);
}

int l_screen_getplayer(lua_State * lua) {
	class Screen * screen = (class Screen *) lua_touserdata(lua, 1);
	if(screen == NULL or not lua_isnumber(lua, 2)) {
		lua_arg_error("screen_getplayer(screen, id)");
	} else {
		int id = lua_tointeger(lua, 2);
		class Player * player = screen->getPlayer(id);
		lua_pushlightuserdata(lua, player);
		return(1);
	}
	return(0);
}

int l_screen_gettopobject(lua_State * lua) {
	class Screen * screen = (class Screen *) lua_touserdata(lua, 1);
	if(
		screen == NULL
		or not lua_isnumber(lua, 2)
		or not lua_isnumber(lua, 3)
	) {
		lua_arg_error("screen_gettopobject(screen, x, y)");
	} else {
		int x = lua_tointeger(lua, 2);
		int y = lua_tointeger(lua, 3);
		lua_pushlightuserdata(lua, screen->getTopObject(x, y));
		return(1);
	}
	return(0);
}

int l_screen_getobject(lua_State * lua) {
	class Screen * screen = (class Screen *) lua_touserdata(lua, 1);
	if(
		screen == NULL
		or not lua_isnumber(lua, 2)
		or not lua_isnumber(lua, 3)
		or not lua_isnumber(lua, 4)
	) {
		lua_arg_error("screen_getobject(screen, x, y, id)");
	} else {
		int x = lua_tointeger(lua, 2);
		int y = lua_tointeger(lua, 3);
		unsigned long int id = lua_tointeger(lua, 4);
		lua_pushlightuserdata(lua, screen->getObject(x, y, id));
		return(1);
	}
	return(0);
}

int l_screen_addobject(lua_State * lua) {
	class Screen * screen = (class Screen *) lua_touserdata(lua, 1);
	class Object * object = (class Object *) lua_touserdata(lua, 4);
	if(
		screen == NULL
		or not lua_isnumber(lua, 2)
		or not lua_isnumber(lua, 3)
		or object == NULL
	) {
		lua_arg_error("screen_addobject(screen, x, y, object)");
	} else {
		int x = lua_tointeger(lua, 2);
		int y = lua_tointeger(lua, 3);
		screen->addObject(x, y, object);
	}
	return(0);
}

int l_screen_remobject(lua_State * lua) {
	class Screen * screen = (class Screen *) lua_touserdata(lua, 1);
	if(
		screen == NULL
		or not lua_isnumber(lua, 2)
		or not lua_isnumber(lua, 3)
		or not lua_isnumber(lua, 4)
	) {
		lua_arg_error("screen_remobject(screen, x, y, id)");
	} else {
		int x = lua_tointeger(lua, 2);
		int y = lua_tointeger(lua, 3);
		unsigned long int id = lua_tointeger(lua, 4);
		screen->remObject(x, y, id);
	}
	return(0);
}

int l_screen_getlandon(lua_State * lua) {
	class Screen * screen = (class Screen *) lua_touserdata(lua, 1);
	if(screen == NULL or not lua_isnumber(lua, 2) or not lua_isnumber(lua, 3)) {
		lua_arg_error("screen_getlandon(screen, x, y)");
	} else {
		unsigned int x = lua_tointeger(lua, 2);
		unsigned int y = lua_tointeger(lua, 3);
		std::string script = screen->getLandOn(x, y);
		lua_pushstring(lua, script.c_str());
		return(1);
	}
	return(0);
}

int l_screen_setlandon(lua_State * lua) {
	class Screen * screen = (class Screen *) lua_touserdata(lua, 1);
	if(screen == NULL or not lua_isnumber(lua, 2) or not lua_isnumber(lua, 3) or not lua_isstring(lua, 4)) {
		lua_arg_error("screen_setlandon(screen, x, y, script)");
	} else {
		unsigned int x = lua_tointeger(lua, 2);
		unsigned int y = lua_tointeger(lua, 3);
		std::string script = lua_tostring(lua, 4);
		screen->setLandOn(x, y, script);
	}
	return(0);
}


int l_screen_resetlandon(lua_State * lua) {
	class Screen * screen = (class Screen *) lua_touserdata(lua, 1);
	if(screen == NULL or not lua_isnumber(lua, 2) or not lua_isnumber(lua, 3)) {
		lua_arg_error("screen_resetlandon(screen, x, y)");
	} else {
		unsigned int x = lua_tointeger(lua, 2);
		unsigned int y = lua_tointeger(lua, 3);
		screen->resetLandOn(x, y);
	}
	return(0);
}

int l_screen_event(lua_State * lua) {
	class Screen * screen = (class Screen *) lua_touserdata(lua, 1);
	if(screen == NULL or not lua_isstring(lua, 2)) {
		lua_arg_error("screen_event(screen, message)");
	} else {
		std::string message = lua_tostring(lua, 2);
		screen->event(message);
	}
	return(0);
}

/* Player */

int l_delete_player(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	if(player == NULL) {
		lua_arg_error("delete_player(player)");
	} else {
		delete(player);
	}
	return(0);
}

int l_player_spawn(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	class Screen * screen = (class Screen *) lua_touserdata(lua, 2);
	if(player == NULL or screen == NULL
			or not lua_isnumber(lua, 3)
			or not lua_isnumber(lua, 4)) {
		lua_arg_error("player_spawn(player)");
	} else {
		int x = lua_tointeger(lua, 3);
		int y = lua_tointeger(lua, 4);
		player->spawn(screen, x, y);
	}
	return(0);
}

int l_player_getid(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	if(player == NULL) {
		lua_arg_error("player_getid(player)");
	} else {
		lua_pushinteger(lua, player->getId());
		return(1);
	}
	return(0);
}

int l_player_getname(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	if(player == NULL) {
		lua_arg_error("player_getname(player)");
	} else {
		lua_pushstring(lua, player->getName().c_str());
		return(1);
	}
	return(0);
}

int l_player_setname(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	if(player == NULL or not lua_isstring(lua, 2)) {
		lua_arg_error("player_setname(player, name)");
	} else {
		std::string name = lua_tostring(lua, 2);
		player->setName(name);
	}
	return(0);
}

int l_player_getaspect(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	if(player == NULL) {
		lua_arg_error("player_getaspect(player)");
	} else {
		lua_pushinteger(lua, player->getAspect());
	}
	return(1);
}

int l_player_setaspect(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	if(player == NULL or not lua_isnumber(lua, 2)) {
		lua_arg_error("player_setaspect(player, aspect)");
	} else {
		Aspect aspect = lua_tointeger(lua, 2);
		player->setAspect(aspect);
	}
	return(0);
}

int l_player_getscreen(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	if(player == NULL) {
		lua_arg_error("player_getscreen(player)");
	} else {
		lua_pushlightuserdata(lua, player->getScreen());
		return(1);
	}
	return(0);
}

int l_player_getx(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	if(player == NULL) {
		lua_arg_error("player_getx(player)");
	} else {
		lua_pushinteger(lua, player->getX());
		return(1);
	}
	return(0);
}

int l_player_gety(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	if(player == NULL) {
		lua_arg_error("player_gety(player)");
	} else {
		lua_pushinteger(lua, player->getY());
		return(1);
	}
	return(0);
}

int l_player_setxy(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	if(player == NULL or not lua_isnumber(lua, 2) or not lua_isnumber(lua, 3)) {
		lua_arg_error("player_setxy(player, x, y)");
	} else {
		int x = lua_tointeger(lua, 2);
		int y = lua_tointeger(lua, 3);
		player->setXY(x, y);
	}
	return(0);
}

int l_player_move(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	if(player == NULL or not lua_isnumber(lua, 2) or not lua_isnumber(lua, 3)) {
		lua_arg_error("player_move(player, x_shift, y_shift)");
	} else {
		int x = lua_tointeger(lua, 2);
		int y = lua_tointeger(lua, 3);
		player->move(x, y);
	}
	return(0);
}

int l_player_changescreen(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	class Screen * screen = (class Screen *) lua_touserdata(lua, 2);
	if(player == NULL or screen == NULL
			or not lua_isnumber(lua, 3) or not lua_isnumber(lua, 4)) {
		lua_arg_error("player_changescreen(player, screen, x, y)");
	} else {
		int x = lua_tointeger(lua, 3);
		int y = lua_tointeger(lua, 4);
		player->changeScreen(screen, x, y);
	}
	return(0);
}

int l_player_getondeath(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	if(player == NULL) {
		lua_arg_error("player_getondeath(player)");
	} else {
		lua_pushstring(lua, player->getOnDeath().c_str());
		return(1);
	}
	return(0);
}

int l_player_setondeath(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	if(player == NULL or not lua_isstring(lua, 2)) {
		lua_arg_error("player_setondeath(player, script)");
	} else {
		std::string script = lua_tostring(lua, 2);
		player->setOnDeath(script);
	}
	return(0);
}

int l_player_getgauge(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	if(player == NULL or not lua_isstring(lua, 2)) {
		lua_arg_error("player_getgauge(player, name)");
	} else {
		std::string name = lua_tostring(lua, 2);
		lua_pushlightuserdata(lua, player->getGauge(name));
		return(1);
	}
	return(0);
}

int l_player_delgauge(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	if(player == NULL or not lua_isstring(lua, 2)) {
		lua_arg_error("player_delgauge(player, name)");
	} else {
		std::string name = lua_tostring(lua, 2);
		player->delGauge(name);
	}
	return(0);
}

int l_player_gettag(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	if(player == NULL or not lua_isstring(lua, 2)) {
		lua_arg_error("player_gettag(player, name)");
	} else {
		std::string name = lua_tostring(lua, 2);
		lua_pushstring(lua, player->getTag(name).c_str());
		return(1);
	}
	return(0);
}

int l_player_settag(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	if(player == NULL or not lua_isstring(lua, 2) or not lua_isstring(lua, 3)) {
		lua_arg_error("player_settag(player, name, value)");
	} else {
		std::string name = lua_tostring(lua, 2);
		std::string value = lua_tostring(lua, 3);
		player->setTag(name, value);
	}
	return(0);
}

int l_player_deltag(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	if(player == NULL or not lua_isstring(lua, 2)) {
		lua_arg_error("player_deltag(player, name)");
	} else {
		std::string name = lua_tostring(lua, 2);
		player->delTag(name);
	}
	return(0);
}

int l_player_getobject(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	if(player == NULL or not lua_isnumber(lua, 2)) {
		lua_arg_error("player_getobject(player, id)");
	} else {
		unsigned long int id = lua_tointeger(lua, 2);
		lua_pushlightuserdata(lua, player->getObject(id));
		return(1);
	}
	return(0);
}

int l_player_addobject(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	class Object * object = (class Object *) lua_touserdata(lua, 2);
	if(player == NULL or object == NULL) {
		lua_arg_error("player_addobject(player, object)");
	} else {
		player->addObject(object);
	}
	return(0);
}

int l_player_remobject(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	if(player == NULL or not lua_isnumber(lua, 2)) {
		lua_arg_error("player_remobject(player, id)");
	} else {
		unsigned long int id = lua_tointeger(lua, 2);
		player->remObject(id);
	}
	return(0);
}

int l_player_isghost(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	if(player == NULL) {
		lua_arg_error("player_isghost(player)");
	} else {
		lua_pushboolean(lua, player->isGhost());
		return(1);
	}
	return(0);
}

int l_player_setghost(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	if(player == NULL) {
		lua_arg_error("player_setghost(player)");
	} else {
		player->setGhost();
	}
	return(0);
}

int l_player_setnotghost(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	if(player == NULL) {
		lua_arg_error("player_setnotghost(player)");
	} else {
		player->setNotGhost();
	}
	return(0);
}

int l_player_message(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	if(player == NULL or not lua_isstring(lua, 2)) {
		lua_arg_error("player_message(player, message)");
	} else {
		std::string message = lua_tostring(lua, 2);
		player->message(message);
	}
	return(0);
}

int l_player_follow(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	class Player * target = (class Player *) lua_touserdata(lua, 2);
	if(player == NULL or target == NULL) {
		lua_arg_error("player_follow(player, target)");
	} else {
		player->follow(target);
	}
	return(0);
}

/* Gauge */

int l_new_gauge(lua_State * lua) {
	class Player * player = (class Player *) lua_touserdata(lua, 1);
	if(player == NULL
			or not lua_isstring(lua, 2)
			or not lua_isnumber(lua, 3)
			or not lua_isnumber(lua, 4)
			or not lua_isnumber(lua, 5)
			or not lua_isnumber(lua, 6)
			) {
		lua_arg_error("new_gauge(player, name, val, max, aspectFull, aspectEmpty, [, visible])");
	} else {
		std::string name = lua_tostring(lua, 2);
		unsigned int val = lua_tointeger(lua, 3);
		unsigned int max = lua_tointeger(lua, 4);
		Aspect aspectFull = lua_tointeger(lua, 5);
		Aspect aspectEmpty = lua_tointeger(lua, 6);
		class Gauge * gauge;
		if(lua_isboolean(lua, 7)) {
			gauge = new Gauge(player, name, val, max, aspectFull, aspectEmpty,
			lua_toboolean(lua, 7));
		} else {
			gauge = new Gauge(player, name, val, max, aspectFull, aspectEmpty);
		}
		lua_pushlightuserdata(lua, gauge);
		return(1);
	}
	return(0);
}

int l_gauge_getname(lua_State * lua) {
	class Gauge * gauge = (class Gauge *) lua_touserdata(lua, 1);
	if(gauge == NULL) {
		lua_arg_error("gauge_getname(gauge)");
	} else {
		lua_pushstring(lua, gauge->getName().c_str());
		return(1);
	}
	return(0);
}

int l_gauge_setname(lua_State * lua) {
	class Gauge * gauge = (class Gauge *) lua_touserdata(lua, 1);
	if(gauge == NULL or not lua_isstring(lua, 2)) {
		lua_arg_error("gauge_setname(lua, name)");
	} else {
		std::string name = lua_tostring(lua, 2);
		gauge->setName(name);
	}
	return(0);
}

int l_gauge_getval(lua_State * lua) {
	class Gauge * gauge = (class Gauge *) lua_touserdata(lua, 1);
	if(gauge == NULL) {
		lua_arg_error("gauge_getval(gauge)");
	} else {
		lua_pushinteger(lua, gauge->getVal());
		return(1);
	}
	return(0);
}

int l_gauge_setval(lua_State * lua) {
	class Gauge * gauge = (class Gauge *) lua_touserdata(lua, 1);
	if(gauge == NULL or not lua_isnumber(lua, 2)) {
		lua_arg_error("gauge_setval(lua, val)");
	} else {
		unsigned int val = lua_tointeger(lua, 2);
		gauge->setVal(val);
	}
	return(0);
}

int l_gauge_increase(lua_State * lua) {
	class Gauge * gauge = (class Gauge *) lua_touserdata(lua, 1);
	if(gauge == NULL or not lua_isnumber(lua, 2)) {
		lua_arg_error("gauge_increase(lua, val)");
	} else {
		unsigned int val = lua_tointeger(lua, 2);
		gauge->increase(val);
	}
	return(0);
}

int l_gauge_decrease(lua_State * lua) {
	class Gauge * gauge = (class Gauge *) lua_touserdata(lua, 1);
	if(gauge == NULL or not lua_isnumber(lua, 2)) {
		lua_arg_error("gauge_decrease(lua, val)");
	} else {
		unsigned int val = lua_tointeger(lua, 2);
		gauge->decrease(val);
	}
	return(0);
}

int l_gauge_getmax(lua_State * lua) {
	class Gauge * gauge = (class Gauge *) lua_touserdata(lua, 1);
	if(gauge == NULL) {
		lua_arg_error("gauge_getmax(gauge)");
	} else {
		lua_pushinteger(lua, gauge->getMax());
		return(1);
	}
	return(0);
}

int l_gauge_setmax(lua_State * lua) {
	class Gauge * gauge = (class Gauge *) lua_touserdata(lua, 1);
	if(gauge == NULL or not lua_isnumber(lua, 2)) {
		lua_arg_error("gauge_setmax(gauge, max)");
	} else {
		unsigned int max = lua_tointeger(lua, 2);
		gauge->setMax(max);
	}
	return(0);
}

int l_gauge_getonfull(lua_State * lua) {
	class Gauge * gauge = (class Gauge *) lua_touserdata(lua, 1);
	if(gauge == NULL) {
		lua_arg_error("gauge_getonfull(gauge)");
	} else {
		lua_pushstring(lua, gauge->getOnFull().c_str());
		return(1);
	}
	return(0);
}

int l_gauge_setonfull(lua_State * lua) {
	class Gauge * gauge = (class Gauge *) lua_touserdata(lua, 1);
	if(gauge == NULL or not lua_isstring(lua, 2)) {
		lua_arg_error("gauge_setonfull(gauge, script)");
	} else {
		std::string script = lua_tostring(lua, 2);
		gauge->setOnFull(script);
	}
	return(0);
}

int l_gauge_resetonfull(lua_State * lua) {
	class Gauge * gauge = (class Gauge *) lua_touserdata(lua, 1);
	if(gauge == NULL) {
		lua_arg_error("gauge_resetonfull(gauge)");
	} else {
		gauge->resetOnFull();
	}
	return(0);
}

int l_gauge_getonempty(lua_State * lua) {
	class Gauge * gauge = (class Gauge *) lua_touserdata(lua, 1);
	if(gauge == NULL) {
		lua_arg_error("gauge_getonempty(gauge)");
	} else {
		lua_pushstring(lua, gauge->getOnEmpty().c_str());
		return(1);
	}
	return(0);
}

int l_gauge_setonempty(lua_State * lua) {
	class Gauge * gauge = (class Gauge *) lua_touserdata(lua, 1);
	if(gauge == NULL or not lua_isstring(lua, 2)) {
		lua_arg_error("gauge_setonempty(gauge, script)");
	} else {
		std::string script = lua_tostring(lua, 2);
		gauge->setOnEmpty(script);
	}
	return(0);
}

int l_gauge_resetonempty(lua_State * lua) {
	class Gauge * gauge = (class Gauge *) lua_touserdata(lua, 1);
	if(gauge == NULL) {
		lua_arg_error("gauge_resetonempty(gauge)");
	} else {
		gauge->resetOnEmpty();
	}
	return(0);
}

int l_gauge_isvisible(lua_State * lua) {
	class Gauge * gauge = (class Gauge *) lua_touserdata(lua, 1);
	if(gauge == NULL) {
		lua_arg_error("gauge_isvisible(gauge)");
	} else {
		lua_pushboolean(lua, gauge->isVisible());
		return(1);
	}
	return(0);
}

int l_gauge_setvisible(lua_State * lua) {
	class Gauge * gauge = (class Gauge *) lua_touserdata(lua, 1);
	if(gauge == NULL) {
		lua_arg_error("gauge_setvisible(gauge)");
	} else {
		gauge->setVisible();
	}
	return(0);
}

int l_gauge_setnotvisible(lua_State * lua) {
	class Gauge * gauge = (class Gauge *) lua_touserdata(lua, 1);
	if(gauge == NULL) {
		lua_arg_error("gauge_setnotvisible(gauge)");
	} else {
		gauge->setNotVisible();
	}
	return(0);
}

/* Object */

int l_new_object(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isnumber(lua, 2)) {
		lua_arg_error("new_object(name, aspect)");
	} else {
		std::string name = lua_tostring(lua, 1);
		Aspect aspect = lua_tointeger(lua, 2);
		class Object * object = new Object(name, aspect);
		lua_pushlightuserdata(lua, object);
		return(1);
	}
	return(0);
}

int l_delete_object(lua_State * lua) {
	class Object * object = (class Object *) lua_touserdata(lua, 1);
	if(object == NULL) {
		lua_arg_error("delete_object(object)");
	} else {
		delete(object);
	}
	return(0);
}

int l_object_getid(lua_State * lua) {
	class Object * object = (class Object *) lua_touserdata(lua, 1);
	if(object == NULL) {
		lua_arg_error("object_getid(object)");
	} else {
		lua_pushinteger(lua, object->getId());
		return(1);
	}
	return(0);
}

int l_object_getname(lua_State * lua) {
	class Object * object = (class Object *) lua_touserdata(lua, 1);
	if(object == NULL) {
		lua_arg_error("object_getname(object)");
	} else {
		lua_pushstring(lua, object->getName().c_str());
		return(1);
	}
	return(0);
}

int l_object_setname(lua_State * lua) {
	class Object * object = (class Object *) lua_touserdata(lua, 1);
	if(object == NULL or not lua_isstring(lua, 2)) {
		lua_arg_error("object_setname(object, name)");
	} else {
		std::string name = lua_tostring(lua, 2);
		object->setName(name);
	}
	return(0);
}

int l_object_getaspect(lua_State * lua) {
	class Object * object = (class Object *) lua_touserdata(lua, 1);
	if(object == NULL) {
		lua_arg_error("object_getaspect(object)");
	} else {
		lua_pushinteger(lua, object->getAspect());
		return(1);
	}
	return(0);
}

int l_object_setaspect(lua_State * lua) {
	class Object * object = (class Object *) lua_touserdata(lua, 1);
	if(object == NULL or not lua_isnumber(lua, 2)) {
		lua_arg_error("object_setaspect(object, aspect)");
	} else {
		Aspect aspect = lua_tointeger(lua, 2);
		object->setAspect(aspect);
	}
	return(0);
}

int l_object_gettag(lua_State * lua) {
	class Object * object = (class Object *) lua_touserdata(lua, 1);
	if(object == NULL or not lua_isstring(lua, 2)) {
		lua_arg_error("object_gettag(object, name)");
	} else {
		std::string name = lua_tostring(lua, 2);
		lua_pushstring(lua, object->getTag(name).c_str());
		return(1);
	}
	return(0);
}

int l_object_settag(lua_State * lua) {
	class Object * object = (class Object *) lua_touserdata(lua, 1);
	if(object == NULL or not lua_isstring(lua, 2) or not lua_isstring(lua, 3)) {
		lua_arg_error("object_settag(object, name, value)");
	} else {
		std::string name = lua_tostring(lua, 2);
		std::string value = lua_tostring(lua, 3);
		object->setTag(name, value);
	}
	return(0);
}

int l_object_deltag(lua_State * lua) {
	class Object * object = (class Object *) lua_touserdata(lua, 1);
	if(object == NULL or not lua_isstring(lua, 2)) {
		lua_arg_error("object_deltag(object, name)");
	} else {
		std::string name = lua_tostring(lua, 2);
		object->delTag(name);
	}
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

	lua_pushlightuserdata(this->lua_state, NULL);
	lua_setglobal(this->lua_state, "NULL");

	lua_register(this->lua_state, "c_rand", l_c_rand);

	lua_register(this->lua_state, "setverbose", l_setverbose);
	lua_register(this->lua_state, "setnoverbose", l_setnoverbose);
	lua_register(this->lua_state, "isverbose", l_isverbose);
	lua_register(this->lua_state, "warning", l_warning);
	lua_register(this->lua_state, "nonfatal", l_nonfatal);
	lua_register(this->lua_state, "fatal", l_fatal);
	lua_register(this->lua_state, "info", l_info);
	lua_register(this->lua_state, "verboseinfo", l_verboseinfo);

	lua_register(this->lua_state, "delete_server", l_delete_server);
	lua_register(this->lua_state, "server_open", l_server_open);
	lua_register(this->lua_state, "server_close", l_server_close);
	lua_register(this->lua_state, "server_isopen", l_server_isopen);
	lua_register(this->lua_state, "server_getport", l_server_getport);
	lua_register(this->lua_state, "server_addscreen", l_server_addscreen);
	lua_register(this->lua_state, "server_getscreen", l_server_getscreen);
	lua_register(this->lua_state, "server_delscreen", l_server_delscreen);
	lua_register(this->lua_state, "server_addtile", l_server_addtile);
	lua_register(this->lua_state, "server_gettile", l_server_gettile);
	lua_register(this->lua_state, "server_getplayer", l_server_getplayer);
	lua_register(this->lua_state, "server_addscript", l_server_addscript);
	lua_register(this->lua_state, "server_getscript", l_server_getscript);
	lua_register(this->lua_state, "server_delscript", l_server_delscript);

	lua_register(this->lua_state, "new_tile", l_new_tile);
	lua_register(this->lua_state, "tile_getid", l_tile_getid);
	lua_register(this->lua_state, "tile_getname", l_tile_getname);
	lua_register(this->lua_state, "tile_setname", l_tile_setname);
	lua_register(this->lua_state, "tile_getaspect", l_tile_getaspect);
	lua_register(this->lua_state, "tile_setaspect", l_tile_setaspect);
	lua_register(this->lua_state, "tile_canland", l_tile_canland);
	lua_register(this->lua_state, "tile_setcanland", l_tile_setcanland);
	lua_register(this->lua_state, "tile_setcantland", l_tile_setcantland);

	lua_register(this->lua_state, "new_screen", l_new_screen);
	lua_register(this->lua_state, "screen_getname", l_screen_getname);
	lua_register(this->lua_state, "screen_setname", l_screen_setname);
	lua_register(this->lua_state, "screen_getwidth", l_screen_getwidth);
	lua_register(this->lua_state, "screen_getheight", l_screen_getheight);
	lua_register(this->lua_state, "screen_gettile", l_screen_gettile);
	lua_register(this->lua_state, "screen_settile", l_screen_settile);
	lua_register(this->lua_state, "screen_getplayer", l_screen_getplayer);
	lua_register(this->lua_state, "screen_gettopobject", l_screen_gettopobject);
	lua_register(this->lua_state, "screen_getobject", l_screen_getobject);
	lua_register(this->lua_state, "screen_addobject", l_screen_addobject);
	lua_register(this->lua_state, "screen_remobject", l_screen_remobject);
	lua_register(this->lua_state, "screen_getlandon", l_screen_getlandon);
	lua_register(this->lua_state, "screen_setlandon", l_screen_setlandon);
	lua_register(this->lua_state, "screen_resetlandon", l_screen_resetlandon);
	lua_register(this->lua_state, "screen_event", l_screen_event);

	lua_register(this->lua_state, "delete_player", l_delete_player);
	lua_register(this->lua_state, "player_spawn", l_player_spawn);
	lua_register(this->lua_state, "player_getid", l_player_getid);
	lua_register(this->lua_state, "player_getname", l_player_getname);
	lua_register(this->lua_state, "player_setname", l_player_setname);
	lua_register(this->lua_state, "player_getaspect", l_player_getaspect);
	lua_register(this->lua_state, "player_setaspect", l_player_setaspect);
	lua_register(this->lua_state, "player_getscreen", l_player_getscreen);
	lua_register(this->lua_state, "player_getx", l_player_getx);
	lua_register(this->lua_state, "player_gety", l_player_gety);
	lua_register(this->lua_state, "player_setxy", l_player_setxy);
	lua_register(this->lua_state, "player_move", l_player_move);
	lua_register(this->lua_state, "player_changescreen", l_player_changescreen);
	lua_register(this->lua_state, "player_getondeath", l_player_getondeath);
	lua_register(this->lua_state, "player_setondeath", l_player_setondeath);
	lua_register(this->lua_state, "player_getgauge", l_player_getgauge);
	lua_register(this->lua_state, "player_delgauge", l_player_delgauge);
	lua_register(this->lua_state, "player_gettag", l_player_gettag);
	lua_register(this->lua_state, "player_settag", l_player_settag);
	lua_register(this->lua_state, "player_deltag", l_player_deltag);
	lua_register(this->lua_state, "player_getobject", l_player_getobject);
	lua_register(this->lua_state, "player_addobject", l_player_addobject);
	lua_register(this->lua_state, "player_remobject", l_player_remobject);
	lua_register(this->lua_state, "player_isghost", l_player_isghost);
	lua_register(this->lua_state, "player_setghost", l_player_setghost);
	lua_register(this->lua_state, "player_setnotghost", l_player_setnotghost);
	lua_register(this->lua_state, "player_message", l_player_message);
	lua_register(this->lua_state, "player_follow", l_player_follow);

	lua_register(this->lua_state, "new_gauge", l_new_gauge);
	lua_register(this->lua_state, "gauge_getname", l_gauge_getname);
	lua_register(this->lua_state, "gauge_setname", l_gauge_setname);
	lua_register(this->lua_state, "gauge_getval", l_gauge_getval);
	lua_register(this->lua_state, "gauge_setval", l_gauge_setval);
	lua_register(this->lua_state, "gauge_increase", l_gauge_increase);
	lua_register(this->lua_state, "gauge_decrease", l_gauge_decrease);
	lua_register(this->lua_state, "gauge_getmax", l_gauge_getmax);
	lua_register(this->lua_state, "gauge_setmax", l_gauge_setmax);
	lua_register(this->lua_state, "gauge_getonfull", l_gauge_getonfull);
	lua_register(this->lua_state, "gauge_setonfull", l_gauge_setonfull);
	lua_register(this->lua_state, "gauge_resetonfull", l_gauge_resetonfull);
	lua_register(this->lua_state, "gauge_getonempty", l_gauge_getonempty);
	lua_register(this->lua_state, "gauge_setonempty", l_gauge_setonempty);
	lua_register(this->lua_state, "gauge_resetonempty", l_gauge_resetonempty);
	lua_register(this->lua_state, "gauge_isvisible", l_gauge_isvisible);
	lua_register(this->lua_state, "gauge_setvisible", l_gauge_setvisible);
	lua_register(this->lua_state, "gauge_setnotvisible", l_gauge_setnotvisible);

	lua_register(this->lua_state, "new_object", l_new_object);
	lua_register(this->lua_state, "delete_object", l_delete_object);
	lua_register(this->lua_state, "object_getid", l_object_getid);
	lua_register(this->lua_state, "object_getname", l_object_getname);
	lua_register(this->lua_state, "object_setname", l_object_setname);
	lua_register(this->lua_state, "object_getaspect", l_object_getaspect);
	lua_register(this->lua_state, "object_setaspect", l_object_setaspect);
	lua_register(this->lua_state, "object_gettag", l_object_gettag);
	lua_register(this->lua_state, "object_settag", l_object_settag);
	lua_register(this->lua_state, "object_deltag", l_object_deltag);

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

