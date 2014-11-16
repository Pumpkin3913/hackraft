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

class Server * Luawrapper::server = NULL;

void lua_arg_error(std::string msg) {
	warning("Lua must call '"+msg+"'.");
}

int l_c_rand(lua_State * lua) {
	if(not lua_isnumber(lua, 1)) {
		lua_arg_error("c_rand(max)");
		lua_pushnil(lua);
	} else {
		static bool done = false;
		if(not done) {
			srand(time(NULL));
			done = true;
		}
		int max = lua_tointeger(lua, 1);
		int x = rand()%max+1;
		lua_pushinteger(lua, x);
	}
	return(1);
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

int l_halt(lua_State * lua) {
	delete(Luawrapper::server);
	return(0);
}

int l_open(lua_State * lua) {
	if(not lua_isnumber(lua, 1)) {
		lua_arg_error("open(port)");
	} else {
		unsigned short int port = lua_tointeger(lua, 2);
		Luawrapper::server->_open(port);
	}
	return(0);
}

int l_close(lua_State * lua) {
	Luawrapper::server->_close();
	return(0);
}

int l_is_open(lua_State * lua) {
	lua_pushboolean(lua, Luawrapper::server->isOpen());
	return(1);
}

int l_get_port(lua_State * lua) {
	lua_pushinteger(lua, Luawrapper::server->getPort());
	return(1);
}

int l_delete_screen(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("delete_screen(screen_id)");
	} else {
		std::string id = lua_tostring(lua, 2);
		Luawrapper::server->delScreen(id);
	}
	return(0);
}

/* Scripts */

int l_add_script(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("add_script(command, filename)");
	} else {
		std::string command = lua_tostring(lua, 2);
		std::string filename = lua_tostring(lua, 3);
		Luawrapper::server->addScript(command, new std::string(filename));
	}
	return(0);
}

int l_get_script(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("get_script(command)");
		lua_pushnil(lua);
	} else {
		std::string command = lua_tostring(lua, 1);
		std::string * script = Luawrapper::server->getScript(command);
		if(script != NULL) {
			lua_pushstring(lua, script->c_str());
		} else {
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_delete_script(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("delete_script(command)");
	} else {
		std::string command = lua_tostring(lua, 1);
		Luawrapper::server->delScript(command);
	}
	return(0);
}

// TODO : list_scripts();

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
		Luawrapper::server->addTile(tile);
	}
	return(0);
}

int l_tile_getname(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("tile_getname(tile_id)");
		lua_pushnil(lua);
	} else {
		std::string id = lua_tostring(lua, 1);
		class Tile * tile = Luawrapper::server->getTile(id);
		if(tile != NULL) {
			lua_pushstring(lua, tile->getName().c_str());
		} else {
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_tile_setname(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("tile_setname(tile_id, name)");
	} else {
		std::string id = lua_tostring(lua, 1);
		class Tile * tile = Luawrapper::server->getTile(id);
		if(tile != NULL) {
			std::string name = lua_tostring(lua, 2);
			tile->setName(name);
		} else {
			warning("Tile '"+id+"' doesn't exist.");
		}
	}
	return(0);
}

int l_tile_getaspect(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("tile_getaspect(tile_id)");
		lua_pushnil(lua);
	} else {
		std::string id = lua_tostring(lua, 1);
		class Tile * tile = Luawrapper::server->getTile(id);
		if(tile != NULL) {
			lua_pushinteger(lua, tile->getAspect());
		} else {
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_tile_setaspect(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isnumber(lua, 2)) {
		lua_arg_error("tile_setaspect(tile_id, aspect)");
	} else {
		std::string id = lua_tostring(lua, 1);
		class Tile * tile = Luawrapper::server->getTile(id);
		if(tile != NULL) {
			Aspect aspect = lua_tointeger(lua, 2);
			tile->setAspect(aspect);
		} else {
			warning("Tile '"+id+"' doesn't exist.");
		}
	}
	return(0);
}

int l_tile_canland(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("tile_canland(tile_id)");
		lua_pushnil(lua);
	} else {
		std::string id = lua_tostring(lua, 1);
		class Tile * tile = Luawrapper::server->getTile(id);
		if(tile != NULL) {
			lua_pushboolean(lua, tile->canLand());
		} else {
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_tile_setcanland(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isboolean(lua, 2)) {
		lua_arg_error("tile_setcanland(tile_id, bool)");
	} else {
		std::string id = lua_tostring(lua, 1);
		class Tile * tile = Luawrapper::server->getTile(id);
		if(tile != NULL) {
			bool b = lua_toboolean(lua, 2);
			b ? tile->setCanLand() : tile->setCantLand();
		} else {
			warning("Tile '"+id+"' doesn't exist.");
		}
	}
	return(0);
}

/* Screen */

int l_new_screen(lua_State * lua) {
	if(not lua_isstring(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isnumber(lua, 3)
			or not lua_isnumber(lua, 4)
			or not lua_isstring(lua, 5)) {
		lua_arg_error("new_screen(screen_id, name, width, height, tile_id)");
	} else {
		std::string screen_id = lua_tostring(lua, 1);
		std::string name = lua_tostring(lua, 2);
		unsigned int width = lua_tointeger(lua, 3);
		unsigned int height = lua_tointeger(lua, 4);
		std::string tile_id = lua_tostring(lua, 5);
		class Tile * tile = Luawrapper::server->getTile(tile_id);
		if(tile != NULL) {
			new Screen(Luawrapper::server,
					screen_id, name, width, height, tile);
		} else {
			warning("Tile '"+tile_id+"' doesn't exist.");
		}
	}
	return(0);
}

int l_screen_getname(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("screen_getname(screen_id)");
		lua_pushnil(lua);
	} else {
		std::string screen_id = lua_tostring(lua, 1);
		class Screen * screen = Luawrapper::server->getScreen(screen_id);
		if(screen != NULL) {
			lua_pushstring(lua, screen->getName().c_str());
		} else {
			warning("Screen '"+screen_id+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_screen_setname(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("screen_setname(screen_id, name)");
	} else {
		std::string screen_id = lua_tostring(lua, 1);
		class Screen * screen = Luawrapper::server->getScreen(screen_id);
		if(screen != NULL) {
			std::string name = lua_tostring(lua, 2);
			screen->setName(name);
		} else {
			warning("Screen '"+screen_id+"' doesn't exist.");
		}
	}
	return(0);
}

int l_screen_getwidth(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("screen_getwidth(screen_id)");
		lua_pushnil(lua);
	} else {
		std::string screen_id = lua_tostring(lua, 1);
		class Screen * screen = Luawrapper::server->getScreen(screen_id);
		if(screen != NULL) {
			lua_pushinteger(lua, screen->getWidth());
		} else {
			warning("Screen '"+screen_id+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_screen_getheight(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("screen_getheight(screen_id)");
		lua_pushnil(lua);
	} else {
		std::string screen_id = lua_tostring(lua, 1);
		class Screen * screen = Luawrapper::server->getScreen(screen_id);
		if(screen != NULL) {
			lua_pushinteger(lua, screen->getHeight());
		} else {
			warning("Screen '"+screen_id+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_screen_gettile(lua_State * lua) {
	if(not lua_isstring(lua, 1)
			or not lua_isnumber(lua, 2)
			or not lua_isnumber(lua, 3)) {
		lua_arg_error("screen_gettile(screen_id, x, y)");
		lua_pushnil(lua);
	} else {
		std::string screen_id = lua_tostring(lua, 1);
		class Screen * screen = Luawrapper::server->getScreen(screen_id);
		if(screen != NULL) {
			unsigned int x = lua_tointeger(lua, 2);
			unsigned int y = lua_tointeger(lua, 3);
			class Tile * tile = screen->getTile(x, y);
			if(tile != NULL) {
				lua_pushstring(lua, tile->getId().c_str());
			} else {
				warning("Invalid place "
					+ std::to_string(x) + "-" + std::to_string(y)
					+ " in screen '" + screen_id + "'.");
				lua_pushnil(lua);
			}
		} else {
			warning("Screen '"+screen_id+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_screen_settile(lua_State * lua) {
	if(not lua_isstring(lua, 1)
			or not lua_isnumber(lua, 2)
			or not lua_isnumber(lua, 3)
			or not lua_isstring(lua, 4)) {
		lua_arg_error("screen_settile(screen_id, x, y, tile_id)");
	} else {
		std::string screen_id = lua_tostring(lua, 1);
		class Screen * screen = Luawrapper::server->getScreen(screen_id);
		if(screen != NULL) {
			std::string tile_id = lua_tostring(lua, 4);
			class Tile * tile = Luawrapper::server->getTile(tile_id);
			if(tile != NULL) {
				unsigned int x = lua_tointeger(lua, 2);
				unsigned int y = lua_tointeger(lua, 3);
				screen->setTile(x, y, tile);
			} else {
				warning("Tile '"+tile_id+"' doesn't exist.");
			}
		} else {
			warning("Screen '"+screen_id+"' doesn't exist.");
		}
	}
	return(0);
}

/* XXX //

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

// XXX */

int l_screen_getlandon(lua_State * lua) {
	if(not lua_isstring(lua, 1)
			or not lua_isnumber(lua, 2)
			or not lua_isnumber(lua, 3)) {
		lua_arg_error("screen_getlandon(screen_id, x, y)");
		lua_pushnil(lua);
	} else {
		std::string screen_id = lua_tostring(lua, 1);
		class Screen * screen = Luawrapper::server->getScreen(screen_id);
		if(screen != NULL) {
			unsigned int x = lua_tointeger(lua, 2);
			unsigned int y = lua_tointeger(lua, 3);
			std::string * script = screen->getLandOn(x, y);
			if(script != NULL) {
				lua_pushstring(lua, script->c_str());
			} else {
				lua_pushnil(lua);
			}
		} else {
			warning("Screen '"+screen_id+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_screen_setlandon(lua_State * lua) {
	if(not lua_isstring(lua, 1)
			or not lua_isnumber(lua, 2)
			or not lua_isnumber(lua, 3)
			or not lua_isstring(lua, 4)) {
		lua_arg_error("screen_setlandon(screen_id, x, y, script)");
	} else {
		std::string screen_id = lua_tostring(lua, 1);
		class Screen * screen = Luawrapper::server->getScreen(screen_id);
		if(screen != NULL) {
			unsigned int x = lua_tointeger(lua, 2);
			unsigned int y = lua_tointeger(lua, 3);
			std::string script = lua_tostring(lua, 4);
			screen->setLandOn(x, y, script);
		} else {
			warning("Screen '"+screen_id+"' doesn't exist.");
		}
	}
	return(0);
}


int l_screen_resetlandon(lua_State * lua) {
	if(not lua_isstring(lua, 1)
			or not lua_isnumber(lua, 2)
			or not lua_isnumber(lua, 3)) {
		lua_arg_error("screen_resetlandon(screen_id, x, y)");
	} else {
		std::string screen_id = lua_tostring(lua, 1);
		class Screen * screen = Luawrapper::server->getScreen(screen_id);
		if(screen != NULL) {
			unsigned int x = lua_tointeger(lua, 2);
			unsigned int y = lua_tointeger(lua, 3);
			screen->resetLandOn(x, y);
		} else {
			warning("Screen '"+screen_id+"' doesn't exist.");
		}
	}
	return(0);
}

int l_screen_event(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("screen_event(screen_id, message)");
	} else {
		std::string screen_id = lua_tostring(lua, 1);
		class Screen * screen = Luawrapper::server->getScreen(screen_id);
		if(screen != NULL) {
			std::string message = lua_tostring(lua, 2);
			screen->event(message);
		} else {
			warning("Screen '"+screen_id+"' doesn't exist.");
		}
	}
	return(0);
}

/* Player */

int l_delete_player(lua_State * lua) {
	if(not lua_isnumber(lua, 1)) {
		lua_arg_error("delete_player(player_id)");
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			delete(player);
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
		}
	}
	return(0);
}

int l_player_spawn(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isnumber(lua, 3)
			or not lua_isnumber(lua, 4)) {
		lua_arg_error("player_spawn(player_id, screen_id, x, y)");
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			std::string screen_id = lua_tostring(lua, 2);
			class Screen * screen = Luawrapper::server->getScreen(screen_id);
			if(screen != NULL) {
				int x = lua_tointeger(lua, 3);
				int y = lua_tointeger(lua, 4);
				player->spawn(screen, x, y);
			} else {
				warning("Screen '"+screen_id+"' doesn't exist.");
			}
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
		}
	}
	return(0);
}

int l_player_getname(lua_State * lua) {
	if(not lua_isnumber(lua, 1)) {
		lua_arg_error("player_getname(player_id)");
		lua_pushnil(lua);
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			lua_pushstring(lua, player->getName().c_str());
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_player_setname(lua_State * lua) {
	if(not lua_isnumber(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("player_setname(player_id, name)");
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			std::string name = lua_tostring(lua, 2);
			player->setName(name);
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
		}
	}
	return(0);
}

int l_player_getaspect(lua_State * lua) {
	if(not lua_isnumber(lua, 1)) {
		lua_arg_error("player_getaspect(player_id)");
		lua_pushnil(lua);
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			lua_pushinteger(lua, player->getAspect());
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_player_setaspect(lua_State * lua) {
	if(not lua_isnumber(lua, 1) or not lua_isnumber(lua, 2)) {
		lua_arg_error("player_setaspect(player_id, aspect)");
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			Aspect aspect = lua_tointeger(lua, 2);
			player->setAspect(aspect);
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
		}
	}
	return(0);
}

int l_player_getscreen(lua_State * lua) {
	if(not lua_isnumber(lua, 1)) {
		lua_arg_error("player_getscreen(player_id)");
		lua_pushnil(lua);
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			lua_pushstring(lua, player->getScreen()->getId().c_str());
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_player_getx(lua_State * lua) {
	if(not lua_isnumber(lua, 1)) {
		lua_arg_error("player_getx(player_id)");
		lua_pushnil(lua);
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			lua_pushinteger(lua, player->getX());
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_player_gety(lua_State * lua) {
	if(not lua_isnumber(lua, 1)) {
		lua_arg_error("player_gety(player_id)");
		lua_pushnil(lua);
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			lua_pushinteger(lua, player->getY());
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_player_setxy(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isnumber(lua, 2)
			or not lua_isnumber(lua, 3)) {
		lua_arg_error("player_setxy(player_id, x, y)");
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			int x = lua_tointeger(lua, 2);
			int y = lua_tointeger(lua, 3);
			player->setXY(x, y);
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
		}
	}
	return(0);
}

int l_player_move(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isnumber(lua, 2)
			or not lua_isnumber(lua, 3)) {
		lua_arg_error("player_move(player_id, x_shift, y_shift)");
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			int x = lua_tointeger(lua, 2);
			int y = lua_tointeger(lua, 3);
			player->move(x, y);
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
		}
	}
	return(0);
}

int l_player_changescreen(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isnumber(lua, 3)
			or not lua_isnumber(lua, 4)) {
		lua_arg_error("player_changescreen(player_id, screen_id, x, y)");
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			std::string screen_id = lua_tostring(lua, 1);
			class Screen * screen = Luawrapper::server->getScreen(screen_id);
			if(screen != NULL) {
				int x = lua_tointeger(lua, 3);
				int y = lua_tointeger(lua, 4);
				player->changeScreen(screen, x, y);
			} else {
				warning("Screen '"+screen_id+"' doesn't exist.");
			}
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
		}
	}
	return(0);
}

int l_player_getondeath(lua_State * lua) {
	if(not lua_isnumber(lua, 1)) {
		lua_arg_error("player_getondeath(player_id)");
		lua_pushnil(lua);
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			lua_pushstring(lua, player->getOnDeath().c_str());
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_player_setondeath(lua_State * lua) {
	if(not lua_isnumber(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("player_setondeath(player_id, script)");
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			std::string script = lua_tostring(lua, 2);
			player->setOnDeath(script);
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
		}
	}
	return(0);
}

/* XXX //
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
// XXX */

int l_player_delgauge(lua_State * lua) {
	if(not lua_isnumber(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("player_delgauge(player_id, gauge_id)");
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			std::string gauge_id = lua_tostring(lua, 2);
			player->delGauge(gauge_id);
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
		}
	}
	return(0);
}

int l_player_gettag(lua_State * lua) {
	if(not lua_isnumber(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("player_gettag(player_id, tag_id)");
		lua_pushnil(lua);
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			std::string tag_id = lua_tostring(lua, 2);
			lua_pushstring(lua, player->getTag(tag_id).c_str());
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_player_settag(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isstring(lua, 3)) {
		lua_arg_error("player_settag(player_id, tag_id, value)");
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			std::string tag_id = lua_tostring(lua, 2);
			std::string value = lua_tostring(lua, 3);
			player->setTag(tag_id, value);
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
		}
	}
	return(0);
}

int l_player_deltag(lua_State * lua) {
	if(not lua_isnumber(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("player_deltag(player_id, tag_id)");
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			std::string tag_id = lua_tostring(lua, 2);
			player->delTag(tag_id);
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
		}
	}
	return(0);
}

/* XXX //

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

// XXX */

int l_player_isghost(lua_State * lua) {
	if(not lua_isnumber(lua, 1)) {
		lua_arg_error("player_isghost(player_id)");
		lua_pushnil(lua);
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			lua_pushboolean(lua, player->isGhost());
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_player_setghost(lua_State * lua) {
	if(not lua_isnumber(lua, 1) or not lua_isboolean(lua, 2)) {
		lua_arg_error("player_setghost(player_id, bool)");
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			bool b = lua_toboolean(lua, 2);
			b ? player->setGhost() : player->setNotGhost();
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
		}
	}
	return(0);
}

int l_player_message(lua_State * lua) {
	if(not lua_isnumber(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("player_message(player_id, message)");
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			std::string message = lua_tostring(lua, 2);
			player->message(message);
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
		}
	}
	return(0);
}

int l_player_follow(lua_State * lua) {
	if(not lua_isnumber(lua, 1) or not lua_isnumber(lua, 2)) {
		lua_arg_error("player_follow(player_id, target_player_id)");
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			int target_id = lua_tointeger(lua, 2);
			class Player * target = Luawrapper::server->getPlayer(target_id);
			if(target != NULL) {
				player->follow(target);
			} else {
				warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
			}
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
		}
	}
	return(0);
}

/* Gauge */

int l_new_gauge(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isnumber(lua, 3)
			or not lua_isnumber(lua, 4)
			or not lua_isnumber(lua, 5)
			or not lua_isnumber(lua, 6)) {
		lua_arg_error("new_gauge(player_id, gauge_id, val, max, aspectFull, aspectEmpty, [, visible])");
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			std::string gauge_id = lua_tostring(lua, 2);
			unsigned int val = lua_tointeger(lua, 3);
			unsigned int max = lua_tointeger(lua, 4);
			Aspect aspectFull = lua_tointeger(lua, 5);
			Aspect aspectEmpty = lua_tointeger(lua, 6);
			if(lua_isboolean(lua, 7)) {
				new Gauge(player, gauge_id, val, max, aspectFull, aspectEmpty,
						lua_toboolean(lua, 7));
			} else {
				new Gauge(player, gauge_id, val, max, aspectFull, aspectEmpty);
			}
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
		}
	}
	return(0);
}

int l_gauge_getname(lua_State * lua) {
	if(not lua_isnumber(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("gauge_getname(player_id, gauge_id)");
		lua_pushnil(lua);
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			std::string gauge_id = lua_tostring(lua, 2);
			class Gauge * gauge = player->getGauge(gauge_id);
			if(gauge != NULL) {
				lua_pushstring(lua, gauge->getName().c_str());
			} else {
				warning("Player '"+std::to_string(player_id)
						+"' doesn't have gauge '"+gauge_id+"'.");
				lua_pushnil(lua);
			}
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_gauge_setname(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isstring(lua, 3)) {
		lua_arg_error("gauge_setname(player_id, gauge_id, name)");
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			std::string gauge_id = lua_tostring(lua, 2);
			class Gauge * gauge = player->getGauge(gauge_id);
			if(gauge != NULL) {
				std::string name = lua_tostring(lua, 2);
				gauge->setName(name);
			} else {
				warning("Player '"+std::to_string(player_id)
						+"' doesn't have gauge '"+gauge_id+"'.");
			}
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
		}
	}
	return(0);
}

int l_gauge_getval(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)) {
		lua_arg_error("gauge_getval(player_id, gauge_id)");
		lua_pushnil(lua);
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			std::string gauge_id = lua_tostring(lua, 2);
			class Gauge * gauge = player->getGauge(gauge_id);
			if(gauge != NULL) {
				lua_pushinteger(lua, gauge->getVal());
			} else {
				warning("Player '"+std::to_string(player_id)
						+"' doesn't have gauge '"+gauge_id+"'.");
				lua_pushnil(lua);
			}
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_gauge_setval(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isnumber(lua, 3)) {
		lua_arg_error("gauge_setval(player_id, gauge_id, val)");
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			std::string gauge_id = lua_tostring(lua, 2);
			class Gauge * gauge = player->getGauge(gauge_id);
			if(gauge != NULL) {
				unsigned int val = lua_tointeger(lua, 3);
				gauge->setVal(val);
			} else {
				warning("Player '"+std::to_string(player_id)
						+"' doesn't have gauge '"+gauge_id+"'.");
			}
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
		}
	}
	return(0);
}

int l_gauge_increase(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isnumber(lua, 3)) {
		lua_arg_error("gauge_increase(player_id, gauge_id, val)");
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			std::string gauge_id = lua_tostring(lua, 2);
			class Gauge * gauge = player->getGauge(gauge_id);
			if(gauge != NULL) {
				unsigned int val = lua_tointeger(lua, 3);
				gauge->increase(val);
			} else {
				warning("Player '"+std::to_string(player_id)
						+"' doesn't have gauge '"+gauge_id+"'.");
			}
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
		}
	}
	return(0);
}

int l_gauge_decrease(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isnumber(lua, 3)) {
		lua_arg_error("gauge_decrease(player_id, gauge_id, val)");
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			std::string gauge_id = lua_tostring(lua, 2);
			class Gauge * gauge = player->getGauge(gauge_id);
			if(gauge != NULL) {
				unsigned int val = lua_tointeger(lua, 3);
				gauge->decrease(val);
			} else {
				warning("Player '"+std::to_string(player_id)
						+"' doesn't have gauge '"+gauge_id+"'.");
			}
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
		}
	}
	return(0);
}

int l_gauge_getmax(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)) {
		lua_arg_error("gauge_getmax(player_id, gauge_id)");
		lua_pushnil(lua);
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			std::string gauge_id = lua_tostring(lua, 2);
			class Gauge * gauge = player->getGauge(gauge_id);
			if(gauge != NULL) {
				lua_pushinteger(lua, gauge->getMax());
			} else {
				warning("Player '"+std::to_string(player_id)
						+"' doesn't have gauge '"+gauge_id+"'.");
				lua_pushnil(lua);
			}
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_gauge_setmax(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isnumber(lua, 3)) {
		lua_arg_error("gauge_setmax(player_id, gauge_id, max)");
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			std::string gauge_id = lua_tostring(lua, 2);
			class Gauge * gauge = player->getGauge(gauge_id);
			if(gauge != NULL) {
				unsigned int val = lua_tointeger(lua, 3);
				gauge->setMax(val);
			} else {
				warning("Player '"+std::to_string(player_id)
						+"' doesn't have gauge '"+gauge_id+"'.");
			}
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
		}
	}
	return(0);
}

int l_gauge_getonfull(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)) {
		lua_arg_error("gauge_getonfull(player_id, gauge_id)");
		lua_pushnil(lua);
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			std::string gauge_id = lua_tostring(lua, 2);
			class Gauge * gauge = player->getGauge(gauge_id);
			if(gauge != NULL) {
				lua_pushstring(lua, gauge->getOnFull().c_str());
			} else {
				warning("Player '"+std::to_string(player_id)
						+"' doesn't have gauge '"+gauge_id+"'.");
				lua_pushnil(lua);
			}
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_gauge_setonfull(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isstring(lua, 3)) {
		lua_arg_error("gauge_setonfull(player_id, gauge_id, script)");
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			std::string gauge_id = lua_tostring(lua, 2);
			class Gauge * gauge = player->getGauge(gauge_id);
			if(gauge != NULL) {
				std::string script = lua_tostring(lua, 3);
				gauge->setOnFull(script);
			} else {
				warning("Player '"+std::to_string(player_id)
						+"' doesn't have gauge '"+gauge_id+"'.");
			}
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
		}
	}
	return(0);
}

int l_gauge_resetonfull(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)) {
		lua_arg_error("gauge_resetonfull(player_id, gauge_id)");
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			std::string gauge_id = lua_tostring(lua, 2);
			class Gauge * gauge = player->getGauge(gauge_id);
			if(gauge != NULL) {
				gauge->resetOnFull();
			} else {
				warning("Player '"+std::to_string(player_id)
						+"' doesn't have gauge '"+gauge_id+"'.");
			}
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
		}
	}
	return(0);
}

int l_gauge_getonempty(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)) {
		lua_arg_error("gauge_getonempty(player_id, gauge_id)");
		lua_pushnil(lua);
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			std::string gauge_id = lua_tostring(lua, 2);
			class Gauge * gauge = player->getGauge(gauge_id);
			if(gauge != NULL) {
				lua_pushstring(lua, gauge->getOnEmpty().c_str());
			} else {
				warning("Player '"+std::to_string(player_id)
						+"' doesn't have gauge '"+gauge_id+"'.");
				lua_pushnil(lua);
			}
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_gauge_setonempty(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isstring(lua, 3)) {
		lua_arg_error("gauge_setonempty(player_id, gauge_id, script)");
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			std::string gauge_id = lua_tostring(lua, 2);
			class Gauge * gauge = player->getGauge(gauge_id);
			if(gauge != NULL) {
				std::string script = lua_tostring(lua, 3);
				gauge->setOnEmpty(script);
			} else {
				warning("Player '"+std::to_string(player_id)
						+"' doesn't have gauge '"+gauge_id+"'.");
			}
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
		}
	}
	return(0);
}

int l_gauge_resetonempty(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)) {
		lua_arg_error("gauge_resetonempty(player_id, gauge_id)");
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			std::string gauge_id = lua_tostring(lua, 2);
			class Gauge * gauge = player->getGauge(gauge_id);
			if(gauge != NULL) {
				gauge->resetOnEmpty();
			} else {
				warning("Player '"+std::to_string(player_id)
						+"' doesn't have gauge '"+gauge_id+"'.");
			}
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
		}
	}
	return(0);
}

int l_gauge_isvisible(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)) {
		lua_arg_error("gauge_isvisible(player_id, gauge_id)");
		lua_pushnil(lua);
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			std::string gauge_id = lua_tostring(lua, 2);
			class Gauge * gauge = player->getGauge(gauge_id);
			if(gauge != NULL) {
				lua_pushboolean(lua, gauge->isVisible());
			} else {
				warning("Player '"+std::to_string(player_id)
						+"' doesn't have gauge '"+gauge_id+"'.");
			}
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
		}
	}
	return(1);
}

int l_gauge_setvisible(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isboolean(lua, 3)) {
		lua_arg_error("gauge_setvisible(player_id, gauge_id, bool)");
	} else {
		int player_id = lua_tointeger(lua, 1);
		class Player * player = Luawrapper::server->getPlayer(player_id);
		if(player != NULL) {
			std::string gauge_id = lua_tostring(lua, 2);
			class Gauge * gauge = player->getGauge(gauge_id);
			if(gauge != NULL) {
				bool b = lua_toboolean(lua, 3);
				b ? gauge->setVisible() : gauge->setNotVisible();
			} else {
				warning("Player '"+std::to_string(player_id)
						+"' doesn't have gauge '"+gauge_id+"'.");
			}
		} else {
			warning("Player '"+std::to_string(player_id)+"' doesn't exist.");
		}
	}
	return(0);
}

/* Object */

/* XXX //

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

// XXX */

/* Wraper class */

Luawrapper::Luawrapper(class Server * server) :
	lua_state(luaL_newstate())
{
	Luawrapper::server = server;
	luaL_openlibs(this->lua_state);

	/* XXX //
	lua_pushlightuserdata(this->lua_state, this->server);
	lua_setglobal(this->lua_state, "Server");

	lua_pushlightuserdata(this->lua_state, NULL);
	lua_setglobal(this->lua_state, "NULL");
	// XXX */

	lua_register(this->lua_state, "c_rand", l_c_rand);

	lua_register(this->lua_state, "setverbose", l_setverbose);
	lua_register(this->lua_state, "setnoverbose", l_setnoverbose);
	lua_register(this->lua_state, "isverbose", l_isverbose);
	lua_register(this->lua_state, "warning", l_warning);
	lua_register(this->lua_state, "nonfatal", l_nonfatal);
	lua_register(this->lua_state, "fatal", l_fatal);
	lua_register(this->lua_state, "info", l_info);
	lua_register(this->lua_state, "verbose", l_verboseinfo);

	lua_register(this->lua_state, "halt", l_halt);
	lua_register(this->lua_state, "open", l_open);
	lua_register(this->lua_state, "close", l_close);
	lua_register(this->lua_state, "is_open", l_is_open);
	lua_register(this->lua_state, "get_port", l_get_port);
	lua_register(this->lua_state, "delete_screen", l_delete_screen);
	lua_register(this->lua_state, "add_script", l_add_script);
	lua_register(this->lua_state, "get_script", l_get_script);
	lua_register(this->lua_state, "delete_script", l_delete_script);

	lua_register(this->lua_state, "new_tile", l_new_tile);
	lua_register(this->lua_state, "tile_getname", l_tile_getname);
	lua_register(this->lua_state, "tile_setname", l_tile_setname);
	lua_register(this->lua_state, "tile_getaspect", l_tile_getaspect);
	lua_register(this->lua_state, "tile_setaspect", l_tile_setaspect);
	lua_register(this->lua_state, "tile_canland", l_tile_canland);
	lua_register(this->lua_state, "tile_setcanland", l_tile_setcanland);

	lua_register(this->lua_state, "new_screen", l_new_screen);
	lua_register(this->lua_state, "screen_getname", l_screen_getname);
	lua_register(this->lua_state, "screen_setname", l_screen_setname);
	lua_register(this->lua_state, "screen_getwidth", l_screen_getwidth);
	lua_register(this->lua_state, "screen_getheight", l_screen_getheight);
	lua_register(this->lua_state, "screen_gettile", l_screen_gettile);
	lua_register(this->lua_state, "screen_settile", l_screen_settile);
	/*
	lua_register(this->lua_state, "screen_getplayer", l_screen_getplayer);
	lua_register(this->lua_state, "screen_gettopobject", l_screen_gettopobject);
	lua_register(this->lua_state, "screen_getobject", l_screen_getobject);
	lua_register(this->lua_state, "screen_addobject", l_screen_addobject);
	lua_register(this->lua_state, "screen_remobject", l_screen_remobject);
	*/
	lua_register(this->lua_state, "screen_getlandon", l_screen_getlandon);
	lua_register(this->lua_state, "screen_setlandon", l_screen_setlandon);
	lua_register(this->lua_state, "screen_resetlandon", l_screen_resetlandon);
	lua_register(this->lua_state, "screen_event", l_screen_event);

	lua_register(this->lua_state, "delete_player", l_delete_player);
	lua_register(this->lua_state, "player_spawn", l_player_spawn);
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
	// lua_register(this->lua_state, "player_getgauge", l_player_getgauge);
	lua_register(this->lua_state, "player_delgauge", l_player_delgauge);
	lua_register(this->lua_state, "player_gettag", l_player_gettag);
	lua_register(this->lua_state, "player_settag", l_player_settag);
	lua_register(this->lua_state, "player_deltag", l_player_deltag);
	/*
	lua_register(this->lua_state, "player_getobject", l_player_getobject);
	lua_register(this->lua_state, "player_addobject", l_player_addobject);
	lua_register(this->lua_state, "player_remobject", l_player_remobject);
	*/
	lua_register(this->lua_state, "player_isghost", l_player_isghost);
	lua_register(this->lua_state, "player_setghost", l_player_setghost);
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

/*
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
*/

	this->executeFile(LUA_INIT_SCRIPT);
}

Luawrapper::~Luawrapper() {
	lua_close(this->lua_state);
}

void Luawrapper::executeFile(std::string filename, class Player * player, std::string arg) {
	if(player) {
		lua_pushinteger(this->lua_state, player->getId());
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

