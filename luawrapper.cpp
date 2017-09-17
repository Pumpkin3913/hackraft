#include "luawrapper.h"

#include "log.h"
#include "gauge.h"
#include "inventory.h"
#include "name.h"
#include "place.h"
#include "character.h"
#include "server.h"
#include "zone.h"

#include <cstdlib> // rand()

class Server * Luawrapper::server = nullptr;

void lua_arg_error(std::string msg) {
	warning("Lua must call '"+msg+"'.");
}

int l_c_rand(lua_State * lua) {
	if(not lua_isnumber(lua, 1)) {
		lua_arg_error("c_rand(max)");
		lua_pushnil(lua);
	} else {
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

int l_info(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("info(message)");
	} else {
		std::string message = lua_tostring(lua, 1);
		info(message);
	}
	return(0);
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

int l_fatal(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("fatal(message)");
	} else {
		std::string message = lua_tostring(lua, 1);
		fatal(message);
	}
	return(0);
}

/* Server */

int l_halt(lua_State * lua) {
	delete(Luawrapper::server);
	return(0);
}

int l_open(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isnumber(lua, 3)
			or not lua_isnumber(lua, 4)) {
		lua_arg_error("open(port, zone, x, y)");
	} else {
		unsigned short int port = lua_tointeger(lua, 1);
		std::string zone { lua_tostring(lua, 2) };
		unsigned int x = lua_tointeger(lua, 3);
		unsigned int y = lua_tointeger(lua, 4);
		Luawrapper::server->_open(port, zone, x, y);
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

int l_delete_zone(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("delete_zone(zone_id)");
	} else {
		std::string id = lua_tostring(lua, 1);
		Luawrapper::server->delZone(id);
	}
	return(0);
}

/* Actions */

int l_add_action(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("add_action(trigger, script)");
	} else {
		std::string trigger = lua_tostring(lua, 1);
		Script script = Script { lua_tostring(lua, 2) };
		Luawrapper::server->addAction(script, trigger);
	}
	return(0);
}

int l_get_action(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("get_action(trigger)");
		lua_pushnil(lua);
	} else {
		std::string trigger = lua_tostring(lua, 1);
		const Script& script = Luawrapper::server->getAction(trigger);
		if(script != Script::noValue) {
			lua_pushstring(lua, script.toString().c_str());
		} else {
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_delete_action(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("delete_action(trigger)");
	} else {
		std::string trigger = lua_tostring(lua, 1);
		Luawrapper::server->delAction(trigger);
	}
	return(0);
}

// TODO : list_scripts();

int l_register_aspect(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isinteger(lua, 2)) {
		lua_arg_error("register_aspect(string, int [, passable])");
	} else {
		Aspect aspect { lua_tostring(lua, 1) };
		int entry = lua_tointeger(lua, 2);
		if(lua_isboolean(lua, 3)) {
			bool default_passable = lua_toboolean(lua, 3);
			Aspect::registerAspect(aspect, entry, default_passable);
		} else {
			Aspect::registerAspect(aspect, entry);
		}
	}
	return(0);
}

/* Timer */

int l_create_timer(lua_State * lua) {
	if(not lua_isnumber(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("create_timer(duration, script)");
		lua_pushnil(lua);
	} else {
		int duration = lua_tointeger(lua, 1);
		Script script { lua_tostring(lua, 2) };
		Uuid id = Luawrapper::server->addTimer(duration, script);
		lua_pushstring(lua, id.toString().c_str());
	}
	return(1);
}

int l_delete_timer(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("delete_timer(timer_id)");
	} else {
		Uuid id { lua_tostring(lua, 1) };
		Luawrapper::server->delTimer(id);
	}
	return(0);
}

int l_timer_getremaining(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("delete_timer(timer_id)");
		lua_pushnil(lua);
	} else {
		Uuid id { lua_tostring(lua, 1) };
		lua_pushinteger(lua, Luawrapper::server->getTimerRemaining(id));
	}
	return(1);
}

int l_timer_setremaining(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isnumber(lua, 2)) {
		lua_arg_error("timer_setremaining(timer_id, val)");
	} else {
		Uuid id { lua_tostring(lua, 1) };
		int remaining = lua_tointeger(lua, 2);
		Luawrapper::server->setTimerRemaining(id, remaining);
	}
	return(0);
}

int l_timer_triggernow(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("timer_triggernow(timer_id)");
	} else {
		Uuid id { lua_tostring(lua, 1) };
		Luawrapper::server->triggerTimer(id);
	}
	return(0);
}

/* Zone */

int l_new_zone(lua_State * lua) {
	if(not lua_isstring(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isnumber(lua, 3)
			or not lua_isnumber(lua, 4)
			or not lua_isstring(lua, 5)) {
		lua_arg_error("new_zone(zone_id, name, width, height, aspect)");
	} else {
		std::string zone_id = lua_tostring(lua, 1);
		Name name { lua_tostring(lua, 2) };
		unsigned int width = lua_tointeger(lua, 3);
		unsigned int height = lua_tointeger(lua, 4);
		Aspect aspect { lua_tostring(lua, 5) };
		new Zone(Luawrapper::server, zone_id, name, width, height, aspect);
	}
	return(0);
}

int l_assert_zone(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("assert_zone(zone_id)");
		lua_pushnil(lua);
	} else {
		std::string zone_id = lua_tostring(lua, 1);
		class Zone * zone = Luawrapper::server->getZone(zone_id);
		if(zone == nullptr) {
			lua_pushboolean(lua, false);
		} else {
			lua_pushboolean(lua, true);
		}
	}
	return(1);
}

int l_zone_getname(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("zone_getname(zone_id)");
		lua_pushnil(lua);
	} else {
		std::string zone_id = lua_tostring(lua, 1);
		class Zone * zone = Luawrapper::server->getZone(zone_id);
		if(zone != nullptr) {
			lua_pushstring(lua, zone->getName().toString().c_str());
		} else {
			warning("Zone '"+zone_id+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_zone_setname(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("zone_setname(zone_id, name)");
	} else {
		std::string zone_id = lua_tostring(lua, 1);
		class Zone * zone = Luawrapper::server->getZone(zone_id);
		if(zone != nullptr) {
			std::string name = lua_tostring(lua, 2);
			zone->setName(Name{name});
		} else {
			warning("Zone '"+zone_id+"' doesn't exist.");
		}
	}
	return(0);
}

int l_zone_getwidth(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("zone_getwidth(zone_id)");
		lua_pushnil(lua);
	} else {
		std::string zone_id = lua_tostring(lua, 1);
		class Zone * zone = Luawrapper::server->getZone(zone_id);
		if(zone != nullptr) {
			lua_pushinteger(lua, zone->getWidth());
		} else {
			warning("Zone '"+zone_id+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_zone_getheight(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("zone_getheight(zone_id)");
		lua_pushnil(lua);
	} else {
		std::string zone_id = lua_tostring(lua, 1);
		class Zone * zone = Luawrapper::server->getZone(zone_id);
		if(zone != nullptr) {
			lua_pushinteger(lua, zone->getHeight());
		} else {
			warning("Zone '"+zone_id+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_zone_event(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("zone_event(zone_id, message)");
	} else {
		std::string zone_id = lua_tostring(lua, 1);
		class Zone * zone = Luawrapper::server->getZone(zone_id);
		if(zone != nullptr) {
			std::string message = lua_tostring(lua, 2);
			zone->event(message);
		} else {
			warning("Zone '"+zone_id+"' doesn't exist.");
		}
	}
	return(0);
}

/* Place */

int l_place_getaspect(lua_State * lua) {
	if(not lua_isstring(lua, 1)
			or not lua_isnumber(lua, 2)
			or not lua_isnumber(lua, 3)) {
		lua_arg_error("place_getaspect(zone_id, x, y)");
		lua_pushnil(lua);
	} else {
		std::string zone_id = lua_tostring(lua, 1);
		class Zone * zone = Luawrapper::server->getZone(zone_id);
		if(zone != nullptr) {
			unsigned int x = lua_tointeger(lua, 2);
			unsigned int y = lua_tointeger(lua, 3);
			class Place * place = zone->getPlace(x, y);
			if(place != nullptr) {
				lua_pushstring(lua, place->getAspect().toString().c_str());
			} else {
				warning("Invalid place "
					+ std::to_string(x) + "-" + std::to_string(y)
					+ " in zone '" + zone_id + "'.");
				lua_pushnil(lua);
			}
		} else {
			warning("Zone '"+zone_id+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_place_setaspect(lua_State * lua) {
	if(not lua_isstring(lua, 1)
			or not lua_isnumber(lua, 2)
			or not lua_isnumber(lua, 3)
			or not lua_isstring(lua, 4)) {
		lua_arg_error("place_setaspect(zone_id, x, y, aspect)");
	} else {
		std::string zone_id = lua_tostring(lua, 1);
		class Zone * zone = Luawrapper::server->getZone(zone_id);
		if(zone != nullptr) {
			unsigned int x = lua_tointeger(lua, 2);
			unsigned int y = lua_tointeger(lua, 3);
			class Place * place = zone->getPlace(x, y);
			if(place != nullptr) {
				// Set aspect.
				Aspect aspect { lua_tostring(lua, 4) };
				place->setAspect(aspect);

				// Set default passability.
				if(Aspect::getAspectDefaultPassable(aspect)) {
					place->setWalkable();
				} else {
					place->setNotWalkable();
				}

				// Update place aspect.
				zone->updatePlaceAspect(x, y);
			} else {
				warning("Invalid place "
					+ std::to_string(x) + "-" + std::to_string(y)
					+ " in zone '" + zone_id + "'.");
				lua_pushnil(lua);
			}
		} else {
			warning("Zone '"+zone_id+"' doesn't exist.");
		}
	}
	return(0);
}

int l_place_ispassable(lua_State * lua) {
	if(not lua_isstring(lua, 1)
			or not lua_isnumber(lua, 2)
			or not lua_isnumber(lua, 3)) {
		lua_arg_error("place_ispassable(zone_id, x, y)");
		lua_pushnil(lua);
	} else {
		std::string zone_id = lua_tostring(lua, 1);
		class Zone * zone = Luawrapper::server->getZone(zone_id);
		if(zone != nullptr) {
			unsigned int x = lua_tointeger(lua, 2);
			unsigned int y = lua_tointeger(lua, 3);
			class Place * place = zone->getPlace(x, y);
			if(place != nullptr) {
				lua_pushboolean(lua, place->isWalkable());
			} else {
				warning("Invalid place "
					+ std::to_string(x) + "-" + std::to_string(y)
					+ " in zone '" + zone_id + "'.");
				lua_pushnil(lua);
			}
		} else {
			warning("Zone '"+zone_id+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_place_setpassable(lua_State * lua) {
	if(not lua_isstring(lua, 1)
			or not lua_isnumber(lua, 2)
			or not lua_isnumber(lua, 3)) {
		lua_arg_error("place_setpassable(zone_id, x, y)");
	} else {
		std::string zone_id = lua_tostring(lua, 1);
		class Zone * zone = Luawrapper::server->getZone(zone_id);
		if(zone != nullptr) {
			unsigned int x = lua_tointeger(lua, 2);
			unsigned int y = lua_tointeger(lua, 3);
			class Place * place = zone->getPlace(x, y);
			if(place != nullptr) {
				place->setWalkable();
			} else {
				warning("Invalid place "
					+ std::to_string(x) + "-" + std::to_string(y)
					+ " in zone '" + zone_id + "'.");
				lua_pushnil(lua);
			}
		} else {
			warning("Zone '"+zone_id+"' doesn't exist.");
		}
	}
	return(0);
}

int l_place_setnotpassable(lua_State * lua) {
	if(not lua_isstring(lua, 1)
			or not lua_isnumber(lua, 2)
			or not lua_isnumber(lua, 3)) {
		lua_arg_error("place_setnotpassable(zone_id, x, y)");
	} else {
		std::string zone_id = lua_tostring(lua, 1);
		class Zone * zone = Luawrapper::server->getZone(zone_id);
		if(zone != nullptr) {
			unsigned int x = lua_tointeger(lua, 2);
			unsigned int y = lua_tointeger(lua, 3);
			class Place * place = zone->getPlace(x, y);
			if(place != nullptr) {
				place->setNotWalkable();
			} else {
				warning("Invalid place "
					+ std::to_string(x) + "-" + std::to_string(y)
					+ " in zone '" + zone_id + "'.");
				lua_pushnil(lua);
			}
		} else {
			warning("Zone '"+zone_id+"' doesn't exist.");
		}
	}
	return(0);
}

int l_place_getlandon(lua_State * lua) {
	if(not lua_isstring(lua, 1)
			or not lua_isnumber(lua, 2)
			or not lua_isnumber(lua, 3)) {
		lua_arg_error("place_getlandon(zone_id, x, y)");
		lua_pushnil(lua);
	} else {
		std::string zone_id = lua_tostring(lua, 1);
		class Zone * zone = Luawrapper::server->getZone(zone_id);
		if(zone != nullptr) {
			unsigned int x = lua_tointeger(lua, 2);
			unsigned int y = lua_tointeger(lua, 3);
			class Place * place = zone->getPlace(x, y);
			if(place != nullptr) {
				lua_pushstring(lua, place->getWhenWalkedOn().toString().c_str());
			} else {
				warning("Invalid place "
					+ std::to_string(x) + "-" + std::to_string(y)
					+ " in zone '" + zone_id + "'.");
				lua_pushnil(lua);
			}
		} else {
			warning("Zone '"+zone_id+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_place_setlandon(lua_State * lua) {
	if(not lua_isstring(lua, 1)
			or not lua_isnumber(lua, 2)
			or not lua_isnumber(lua, 3)
			or not lua_isstring(lua, 4)) {
		lua_arg_error("place_setlandon(zone_id, x, y, script)");
	} else {
		std::string zone_id = lua_tostring(lua, 1);
		class Zone * zone = Luawrapper::server->getZone(zone_id);
		if(zone != nullptr) {
			unsigned int x = lua_tointeger(lua, 2);
			unsigned int y = lua_tointeger(lua, 3);
			class Place * place = zone->getPlace(x, y);
			if(place != nullptr) {
				Script script { lua_tostring(lua, 4) };
				place->setWhenWalkedOn(script);
			} else {
				warning("Invalid place "
					+ std::to_string(x) + "-" + std::to_string(y)
					+ " in zone '" + zone_id + "'.");
				lua_pushnil(lua);
			}
		} else {
			warning("Zone '"+zone_id+"' doesn't exist.");
		}
	}
	return(0);
}

int l_place_resetlandon(lua_State * lua) {
	if(not lua_isstring(lua, 1)
			or not lua_isnumber(lua, 2)
			or not lua_isnumber(lua, 3)) {
		lua_arg_error("place_resetlandon(zone_id, x, y)");
	} else {
		std::string zone_id = lua_tostring(lua, 1);
		class Zone * zone = Luawrapper::server->getZone(zone_id);
		if(zone != nullptr) {
			unsigned int x = lua_tointeger(lua, 2);
			unsigned int y = lua_tointeger(lua, 3);
			class Place * place = zone->getPlace(x, y);
			if(place != nullptr) {
				place->resetWhenWalkedOn();
			} else {
				warning("Invalid place "
					+ std::to_string(x) + "-" + std::to_string(y)
					+ " in zone '" + zone_id + "'.");
				lua_pushnil(lua);
			}
		} else {
			warning("Zone '"+zone_id+"' doesn't exist.");
		}
	}
	return(0);
}

int l_place_gettag(lua_State * lua) {
	if(not lua_isstring(lua, 1)
			or not lua_isnumber(lua, 2)
			or not lua_isnumber(lua, 3)
			or not lua_isstring(lua, 3)) {
		lua_arg_error("place_gettag(zone_id, x, y, tag_id)");
		lua_pushnil(lua);
	} else {
		std::string zone_id = lua_tostring(lua, 1);
		class Zone * zone = Luawrapper::server->getZone(zone_id);
		if(zone != nullptr) {
			int x = lua_tointeger(lua, 2);
			int y = lua_tointeger(lua, 3);
			Place * place = zone->getPlace(x, y);
			if(place != nullptr) {
				TagID tag_id { lua_tostring(lua, 4) };
				lua_pushstring(lua, place->getTag(tag_id).toString().c_str());
			} else {
				warning("Invalid place "
					+ std::to_string(x) + "-" + std::to_string(y)
					+ " in zone '" + zone_id + "'.");
				lua_pushnil(lua);
			}
		} else {
			warning("Zone '"+zone_id+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_place_settag(lua_State * lua) {
	if(not lua_isstring(lua, 1)
			or not lua_isnumber(lua, 2)
			or not lua_isnumber(lua, 3)
			or not lua_isstring(lua, 4)
			or not lua_isstring(lua, 5)) {
		lua_arg_error("place_settag(zone_id, x, y, tag_id, value)");
	} else {
		std::string zone_id = lua_tostring(lua, 1);
		class Zone * zone = Luawrapper::server->getZone(zone_id);
		if(zone != nullptr) {
			int x = lua_tointeger(lua, 2);
			int y = lua_tointeger(lua, 3);
			Place * place = zone->getPlace(x, y);
			if(place != nullptr) {
				TagID tag_id = TagID { lua_tostring(lua, 4) };
				TagValue value = TagValue { lua_tostring(lua, 5) };
				place->setTag(tag_id, value);
			} else {
				warning("Invalid place "
					+ std::to_string(x) + "-" + std::to_string(y)
					+ " in zone '" + zone_id + "'.");
				lua_pushnil(lua);
			}
		} else {
			warning("Zone '"+zone_id+"' doesn't exist.");
		}
	}
	return(0);
}

int l_place_deltag(lua_State * lua) {
	if(not lua_isstring(lua, 1)
			or not lua_isnumber(lua, 2)
			or not lua_isnumber(lua, 3)
			or not lua_isstring(lua, 4)) {
		lua_arg_error("place_deltag(zone_id, x, y, tag_id)");
	} else {
		std::string zone_id = lua_tostring(lua, 1);
		class Zone * zone = Luawrapper::server->getZone(zone_id);
		if(zone != nullptr) {
			int x = lua_tointeger(lua, 2);
			int y = lua_tointeger(lua, 3);
			Place * place = zone->getPlace(x, y);
			if(place != nullptr) {
				TagID tag_id = TagID { lua_tostring(lua, 4) };
				place->delTag(tag_id);
			} else {
				warning("Invalid place "
					+ std::to_string(x) + "-" + std::to_string(y)
					+ " in zone '" + zone_id + "'.");
				lua_pushnil(lua);
			}
		} else {
			warning("Zone '"+zone_id+"' doesn't exist.");
		}
	}
	return(0);
}

/* Character */

int l_delete_character(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("delete_character(character_id)");
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			delete(character);
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
		}
	}
	return(0);
}

int l_assert_character(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("assert_character(character_id)");
		lua_pushnil(lua);
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character == nullptr) {
			lua_pushboolean(lua, false);
		} else {
			lua_pushboolean(lua, true);
		}
	}
	return(1);
}

int l_character_getname(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("character_getname(character_id)");
		lua_pushnil(lua);
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			lua_pushstring(lua, character->getName().toString().c_str());
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_character_setname(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("character_setname(character_id, name)");
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			std::string name = lua_tostring(lua, 2);
			character->setName(Name{name});
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
		}
	}
	return(0);
}

int l_character_getaspect(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("character_getaspect(character_id)");
		lua_pushnil(lua);
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			lua_pushstring(lua, character->getAspect().toString().c_str());
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_character_setaspect(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("character_setaspect(character_id, aspect)");
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			Aspect aspect { lua_tostring(lua, 2) };
			character->setAspect(aspect);

			// Update aspect.
			class Zone * zone = character->getZone();
			if(zone != nullptr) {
				zone->updateCharacter(character);
			}
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
		}
	}
	return(0);
}

int l_character_getzone(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("character_getzone(character_id)");
		lua_pushnil(lua);
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			lua_pushstring(lua, character->getZone()->getId().c_str());
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_character_getx(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("character_getx(character_id)");
		lua_pushnil(lua);
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			lua_pushinteger(lua, character->getX());
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_character_gety(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("character_gety(character_id)");
		lua_pushnil(lua);
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			lua_pushinteger(lua, character->getY());
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_character_setxy(lua_State * lua) {
	if(not lua_isstring(lua, 1)
			or not lua_isnumber(lua, 2)
			or not lua_isnumber(lua, 3)) {
		lua_arg_error("character_setxy(character_id, x, y)");
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			int x = lua_tointeger(lua, 2);
			int y = lua_tointeger(lua, 3);
			character->setXY(x, y);
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
		}
	}
	return(0);
}

int l_character_move(lua_State * lua) {
	if(not lua_isstring(lua, 1)
			or not lua_isnumber(lua, 2)
			or not lua_isnumber(lua, 3)) {
		lua_arg_error("character_move(character_id, x_shift, y_shift)");
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			int x = lua_tointeger(lua, 2);
			int y = lua_tointeger(lua, 3);
			character->move(x, y);
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
		}
	}
	return(0);
}

int l_character_changezone(lua_State * lua) {
	if(not lua_isstring(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isnumber(lua, 3)
			or not lua_isnumber(lua, 4)) {
		lua_arg_error("character_changezone(character_id, zone_id, x, y)");
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			std::string zone_id = lua_tostring(lua, 2);
			class Zone * zone = Luawrapper::server->getZone(zone_id);
			if(zone != nullptr) {
				int x = lua_tointeger(lua, 3);
				int y = lua_tointeger(lua, 4);
				character->changeZone(zone, x, y);
			} else {
				warning("Zone '"+zone_id+"' doesn't exist.");
			}
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
		}
	}
	return(0);
}

int l_character_getwhendeath(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("character_getwhendeath(character_id)");
		lua_pushnil(lua);
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			lua_pushstring(lua, character->getWhenDeath().toString().c_str());
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_character_setwhendeath(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("character_setwhendeath(character_id, script)");
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			Script script = Script { lua_tostring(lua, 2) };
			character->setWhenDeath(script);
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
		}
	}
	return(0);
}

int l_character_delgauge(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("character_delgauge(character_id, gauge_id)");
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			Name name = Name { lua_tostring(lua, 2) };
			character->delGauge(name);
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
		}
	}
	return(0);
}

int l_character_gettag(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("character_gettag(character_id, tag_id)");
		lua_pushnil(lua);
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			TagID tag_id = TagID { lua_tostring(lua, 2) };
			lua_pushstring(lua, character->getTag(tag_id).toString().c_str());
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_character_settag(lua_State * lua) {
	if(not lua_isstring(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isstring(lua, 3)) {
		lua_arg_error("character_settag(character_id, tag_id, value)");
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			TagID tag_id = TagID { lua_tostring(lua, 2) };
			TagValue value = TagValue { lua_tostring(lua, 3) };
			character->setTag(tag_id, value);
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
		}
	}
	return(0);
}

int l_character_deltag(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("character_deltag(character_id, tag_id)");
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			TagID tag_id = TagID { lua_tostring(lua, 2) };
			character->delTag(tag_id);
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
		}
	}
	return(0);
}

int l_character_isghost(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("character_isghost(character_id)");
		lua_pushnil(lua);
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			lua_pushboolean(lua, character->isGhost());
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_character_setghost(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isboolean(lua, 2)) {
		lua_arg_error("character_setghost(character_id, bool)");
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			bool b = lua_toboolean(lua, 2);
			b ? character->setGhost() : character->setNotGhost();
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
		}
	}
	return(0);
}

int l_character_message(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("character_message(character_id, message)");
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			std::string message = lua_tostring(lua, 2);
			character->message(message);
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
		}
	}
	return(0);
}

int l_character_follow(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("character_follow(character_id, target_character_id)");
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			Uuid target_id { lua_tostring(lua, 2) };
			class Character * target = Luawrapper::server->getCharacter(target_id);
			if(target != nullptr) {
				character->follow(target);
			} else {
				warning("Character '"+target_id.toString()+"' doesn't exist.");
			}
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
		}
	}
	return(0);
}

int l_character_hint(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2) or not lua_isstring(lua, 3)) {
		lua_arg_error("character_hint(character_id, aspect, hint)");
		return(0);
	}

	Uuid character_id { lua_tostring(lua, 1) };
	class Character * character = Luawrapper::server->getCharacter(character_id);
	if(character == nullptr) {
		warning("Character '"+character_id.toString()+"' doesn't exist.");
		return(0);
	}

	Aspect aspect { lua_tostring(lua, 2) };

	character->hint(aspect, lua_tostring(lua, 3));

	return(0);
}

/* Gauge */

int l_new_gauge(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isnumber(lua, 3)
			or not lua_isnumber(lua, 4)
			or not lua_isstring(lua, 5)
			or not lua_isstring(lua, 6)) {
		lua_arg_error("new_gauge(character_id, gauge_id, val, max, aspectFull, aspectEmpty, [, visible])");
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			std::string gauge_id = lua_tostring(lua, 2);
			unsigned int val = lua_tointeger(lua, 3);
			unsigned int max = lua_tointeger(lua, 4);
			Aspect aspectFull { lua_tostring(lua, 5) };
			Aspect aspectEmpty { lua_tostring(lua, 6) };
			if(lua_isboolean(lua, 7)) {
				new Gauge(character, Name{gauge_id}, val, max, aspectFull, aspectEmpty,
						lua_toboolean(lua, 7));
			} else {
				new Gauge(character, Name{gauge_id}, val, max, aspectFull, aspectEmpty);
			}
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
		}
	}
	return(0);
}

int l_assert_gauge(lua_State * lua) {
	if(not lua_isnumber(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("assert_gauge(character_id, gauge_id)");
		lua_pushnil(lua);
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			Name name = Name { lua_tostring(lua, 2) };
			class Gauge * gauge = character->getGauge(name);
			if(gauge == nullptr) {
				lua_pushboolean(lua, false);
			} else {
				lua_pushboolean(lua, true);
			}
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_gauge_getname(lua_State * lua) {
	if(not lua_isnumber(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("gauge_getname(character_id, gauge_id)");
		lua_pushnil(lua);
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			Name name = Name { lua_tostring(lua, 2) };
			class Gauge * gauge = character->getGauge(name);
			if(gauge != nullptr) {
				lua_pushstring(lua, gauge->getName().toString().c_str());
			} else {
				warning("Character '"+character_id.toString()
						+"' doesn't have gauge '"+name.toString()+"'.");
				lua_pushnil(lua);
			}
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_gauge_setname(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isstring(lua, 3)) {
		lua_arg_error("gauge_setname(character_id, gauge_id, name)");
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			Name name = Name { lua_tostring(lua, 2) };
			class Gauge * gauge = character->getGauge(name);
			if(gauge != nullptr) {
				Name new_name = Name { lua_tostring(lua, 3) };
				gauge->setName(new_name);
			} else {
				warning("Character '"+character_id.toString()
						+"' doesn't have gauge '"+name.toString()+"'.");
			}
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
		}
	}
	return(0);
}

int l_gauge_getval(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)) {
		lua_arg_error("gauge_getval(character_id, gauge_id)");
		lua_pushnil(lua);
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			Name name = Name { lua_tostring(lua, 2) };
			class Gauge * gauge = character->getGauge(name);
			if(gauge != nullptr) {
				lua_pushinteger(lua, gauge->getVal());
			} else {
				warning("Character '"+character_id.toString()
						+"' doesn't have gauge '"+name.toString()+"'.");
				lua_pushnil(lua);
			}
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_gauge_setval(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isnumber(lua, 3)) {
		lua_arg_error("gauge_setval(character_id, gauge_id, val)");
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			Name name = Name { lua_tostring(lua, 2) };
			class Gauge * gauge = character->getGauge(name);
			if(gauge != nullptr) {
				unsigned int val = lua_tointeger(lua, 3);
				gauge->setVal(val);
			} else {
				warning("Character '"+character_id.toString()
						+"' doesn't have gauge '"+name.toString()+"'.");
			}
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
		}
	}
	return(0);
}

int l_gauge_increase(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isnumber(lua, 3)) {
		lua_arg_error("gauge_increase(character_id, gauge_id, val)");
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			Name name = Name { lua_tostring(lua, 2) };
			class Gauge * gauge = character->getGauge(name);
			if(gauge != nullptr) {
				unsigned int val = lua_tointeger(lua, 3);
				gauge->increase(val);
			} else {
				warning("Character '"+character_id.toString()
						+"' doesn't have gauge '"+name.toString()+"'.");
			}
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
		}
	}
	return(0);
}

int l_gauge_decrease(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isnumber(lua, 3)) {
		lua_arg_error("gauge_decrease(character_id, gauge_id, val)");
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			Name name = Name { lua_tostring(lua, 2) };
			class Gauge * gauge = character->getGauge(name);
			if(gauge != nullptr) {
				unsigned int val = lua_tointeger(lua, 3);
				gauge->decrease(val);
			} else {
				warning("Character '"+character_id.toString()
						+"' doesn't have gauge '"+name.toString()+"'.");
			}
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
		}
	}
	return(0);
}

int l_gauge_getmax(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)) {
		lua_arg_error("gauge_getmax(character_id, gauge_id)");
		lua_pushnil(lua);
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			Name name = Name { lua_tostring(lua, 2) };
			class Gauge * gauge = character->getGauge(name);
			if(gauge != nullptr) {
				lua_pushinteger(lua, gauge->getMax());
			} else {
				warning("Character '"+character_id.toString()
						+"' doesn't have gauge '"+name.toString()+"'.");
				lua_pushnil(lua);
			}
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_gauge_setmax(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isnumber(lua, 3)) {
		lua_arg_error("gauge_setmax(character_id, gauge_id, max)");
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			Name name = Name { lua_tostring(lua, 2) };
			class Gauge * gauge = character->getGauge(name);
			if(gauge != nullptr) {
				unsigned int val = lua_tointeger(lua, 3);
				gauge->setMax(val);
			} else {
				warning("Character '"+character_id.toString()
						+"' doesn't have gauge '"+name.toString()+"'.");
			}
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
		}
	}
	return(0);
}

int l_gauge_getwhenfull(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)) {
		lua_arg_error("gauge_getwhenfull(character_id, gauge_id)");
		lua_pushnil(lua);
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			Name name = Name { lua_tostring(lua, 2) };
			class Gauge * gauge = character->getGauge(name);
			if(gauge != nullptr) {
				lua_pushstring(lua, gauge->getWhenFull().toString().c_str());
			} else {
				warning("Character '"+character_id.toString()
						+"' doesn't have gauge '"+name.toString()+"'.");
				lua_pushnil(lua);
			}
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_gauge_setwhenfull(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isstring(lua, 3)) {
		lua_arg_error("gauge_setwhenfull(character_id, gauge_id, script)");
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			Name name = Name { lua_tostring(lua, 2) };
			class Gauge * gauge = character->getGauge(name);
			if(gauge != nullptr) {
				Script script = Script { lua_tostring(lua, 3) };
				gauge->setWhenFull(script);
			} else {
				warning("Character '"+character_id.toString()
						+"' doesn't have gauge '"+name.toString()+"'.");
			}
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
		}
	}
	return(0);
}

int l_gauge_resetwhenfull(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)) {
		lua_arg_error("gauge_resetwhenfull(character_id, gauge_id)");
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			Name name = Name { lua_tostring(lua, 2) };
			class Gauge * gauge = character->getGauge(name);
			if(gauge != nullptr) {
				gauge->resetWhenFull();
			} else {
				warning("Character '"+character_id.toString()
						+"' doesn't have gauge '"+name.toString()+"'.");
			}
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
		}
	}
	return(0);
}

int l_gauge_getwhenempty(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)) {
		lua_arg_error("gauge_getwhenempty(character_id, gauge_id)");
		lua_pushnil(lua);
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			Name name = Name { lua_tostring(lua, 2) };
			class Gauge * gauge = character->getGauge(name);
			if(gauge != nullptr) {
				lua_pushstring(lua, gauge->getWhenEmpty().toString().c_str());
			} else {
				warning("Character '"+character_id.toString()
						+"' doesn't have gauge '"+name.toString()+"'.");
				lua_pushnil(lua);
			}
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
			lua_pushnil(lua);
		}
	}
	return(1);
}

int l_gauge_setwhenempty(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isstring(lua, 3)) {
		lua_arg_error("gauge_setwhenempty(character_id, gauge_id, script)");
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			Name name = Name { lua_tostring(lua, 2) };
			class Gauge * gauge = character->getGauge(name);
			if(gauge != nullptr) {
				Script script = Script { lua_tostring(lua, 3) };
				gauge->setWhenEmpty(script);
			} else {
				warning("Character '"+character_id.toString()
						+"' doesn't have gauge '"+name.toString()+"'.");
			}
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
		}
	}
	return(0);
}

int l_gauge_resetwhenempty(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)) {
		lua_arg_error("gauge_resetwhenempty(character_id, gauge_id)");
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			Name name = Name { lua_tostring(lua, 2) };
			class Gauge * gauge = character->getGauge(name);
			if(gauge != nullptr) {
				gauge->resetWhenEmpty();
			} else {
				warning("Character '"+character_id.toString()
						+"' doesn't have gauge '"+name.toString()+"'.");
			}
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
		}
	}
	return(0);
}

int l_gauge_isvisible(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)) {
		lua_arg_error("gauge_isvisible(character_id, gauge_id)");
		lua_pushnil(lua);
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			Name name = Name { lua_tostring(lua, 2) };
			class Gauge * gauge = character->getGauge(name);
			if(gauge != nullptr) {
				lua_pushboolean(lua, gauge->isVisible());
			} else {
				warning("Character '"+character_id.toString()
						+"' doesn't have gauge '"+name.toString()+"'.");
			}
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
		}
	}
	return(1);
}

int l_gauge_setvisible(lua_State * lua) {
	if(not lua_isnumber(lua, 1)
			or not lua_isstring(lua, 2)
			or not lua_isboolean(lua, 3)) {
		lua_arg_error("gauge_setvisible(character_id, gauge_id, bool)");
	} else {
		Uuid character_id { lua_tostring(lua, 1) };
		class Character * character = Luawrapper::server->getCharacter(character_id);
		if(character != nullptr) {
			Name name = Name { lua_tostring(lua, 2) };
			class Gauge * gauge = character->getGauge(name);
			if(gauge != nullptr) {
				bool b = lua_toboolean(lua, 3);
				b ? gauge->setVisible() : gauge->setNotVisible();
			} else {
				warning("Character '"+character_id.toString()
						+"' doesn't have gauge '"+name.toString()+"'.");
			}
		} else {
			warning("Character '"+character_id.toString()+"' doesn't exist.");
		}
	}
	return(0);
}

/* Inventory */

int l_create_inventory(lua_State * lua) {
	if(not lua_isinteger(lua, 1)) {
		lua_arg_error("create_inventory(size, [type])");
		lua_pushnil(lua);
		return(1);
	}

	unsigned int size = lua_tointeger(lua, 1);
	std::unique_ptr<Inventory> inventory;
	if(lua_isstring(lua, 2)) {
		std::string type = std::string{ lua_tostring(lua, 2) };
		inventory = std::make_unique<Inventory>(size, type);
	} else {
		inventory = std::make_unique<Inventory>(size);
	}
	Uuid id = Luawrapper::server->addInventory(std::move(inventory));
	lua_pushstring(lua, id.toString().c_str());
	return(1);
}

int l_delete_inventory(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("delete_inventory(inventory_id)");
		return(0);
	}

	Uuid id { lua_tostring(lua, 1) };
	Luawrapper::server->delInventory(id);
	return(0);
}

int l_inventory_size(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("inventory_size(inventory_id)");
		lua_pushnil(lua);
		return(1);
	}

	Uuid id { lua_tostring(lua, 1) };
	Inventory* inventory = Luawrapper::server->getInventory(id);
	if(inventory == nullptr) {
		warning("Inventory "+id.toString()+" doesn't exist.");
		lua_pushnil(lua);
		return(1);
	}

	unsigned int size = inventory->size();
	lua_pushinteger(lua, size);
	return(1);
}

int l_inventory_resize(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isinteger(lua, 2)) {
		lua_arg_error("inventory_resize(inventory_id, size)");
		return(0);
	}

	Uuid id { lua_tostring(lua, 1) };
	Inventory* inventory = Luawrapper::server->getInventory(id);
	if(inventory == nullptr) {
		warning("Inventory "+id.toString()+" doesn't exist.");
		return(0);
	}

	unsigned int size = lua_tointeger(lua, 2);
	inventory->resize(size);
	return(0);
}

int l_inventory_total(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("inventory_total(inventory_id)");
		lua_pushnil(lua);
		return(1);
	}

	Uuid id { lua_tostring(lua, 1) };
	Inventory* inventory = Luawrapper::server->getInventory(id);
	if(inventory == nullptr) {
		warning("Inventory "+id.toString()+" doesn't exist.");
		lua_pushnil(lua);
		return(1);
	}

	lua_pushinteger(lua, inventory->total());
	return(1);
}

int l_inventory_available(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("inventory_available(inventory_id)");
		lua_pushnil(lua);
		return(1);
	}

	Uuid id { lua_tostring(lua, 1) };
	Inventory* inventory = Luawrapper::server->getInventory(id);
	if(inventory == nullptr) {
		warning("Inventory "+id.toString()+" doesn't exist.");
		lua_pushnil(lua);
		return(1);
	}

	lua_pushinteger(lua, inventory->available());
	return(1);
}

int l_inventory_get(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("inventory_get(inventory_id, item_name)");
		lua_pushnil(lua);
		return(1);
	}

	Uuid id { lua_tostring(lua, 1) };
	Inventory* inventory = Luawrapper::server->getInventory(id);
	if(inventory == nullptr) {
		warning("Inventory "+id.toString()+" doesn't exist.");
		lua_pushnil(lua);
		return(1);
	}

	Name name { lua_tostring(lua, 2) };
	std::vector<Uuid> ids = inventory->get_all(name);

	lua_newtable(lua);
	for(int i=0; i<ids.size(); i++) {
		lua_pushstring(lua, ids[i].toString().c_str());
		lua_rawseti(lua, -2, i);
	}
	return(1);
}

int l_inventory_get_all(lua_State * lua) {
	if(not lua_isstring(lua, 1)) {
		lua_arg_error("inventory_get_all(inventory_id)");
		lua_pushnil(lua);
		return(1);
	}

	Uuid id { lua_tostring(lua, 1) };
	Inventory* inventory = Luawrapper::server->getInventory(id);
	if(inventory == nullptr) {
		warning("Inventory "+id.toString()+" doesn't exist.");
		lua_pushnil(lua);
		return(1);
	}

	std::vector<Uuid> ids = inventory->get_all();

	lua_newtable(lua);
	for(int i=0; i<ids.size(); i++) {
		lua_pushstring(lua, ids[i].toString().c_str());
		lua_rawseti(lua, -2, i+1);
	}
	return(1);
}

/* Artifacts */

int l_create_artifact(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2) or not lua_isstring(lua, 3)) {
		lua_arg_error("create_artifact(inventory_id, item_name, item_type, [quantity])");
		lua_pushnil(lua);
		return(1);
	}

	Uuid inv_id { lua_tostring(lua, 1) };
	Inventory* inventory = Luawrapper::server->getInventory(inv_id);
	if(inventory == nullptr) {
		warning("Inventory "+inv_id.toString()+" doesn't exist.");
		lua_pushnil(lua);
		return(1);
	}

	Name name { lua_tostring(lua, 2) };
	std::string type { lua_tostring(lua, 3) };
	std::unique_ptr<Artifact> artifact;
	if(lua_isinteger(lua, 4)) {
		unsigned int quantity = lua_tointeger(lua, 4);
		artifact = std::make_unique<Artifact>(name, type, quantity);
	} else {
		artifact = std::make_unique<Artifact>(name, type);
	}

	std::optional<Uuid> art_id = inventory->add(std::move(artifact));
	if(art_id) {
		lua_pushstring(lua, art_id->toString().c_str());
	} else {
		lua_pushnil(lua);
	}
	return(1);
}

int l_delete_artifact(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("delete_artifact(inventory_id, artifact_id, [quantity])");
		lua_pushnil(lua);
		return(1);
	}

	Uuid inv_id { lua_tostring(lua, 1) };
	Inventory* inventory = Luawrapper::server->getInventory(inv_id);

	if(inventory == nullptr) {
		warning("Inventory "+inv_id.toString()+" doesn't exist.");
		lua_pushnil(lua);
		return(1);
	}

	Uuid art_id { lua_tostring(lua, 2) };
	bool b;
	if(lua_isnumber(lua, 3)) {
		unsigned int quantity = lua_tointeger(lua, 3);
		b = inventory->del(art_id, quantity);
	} else {
		b = inventory->del(art_id);
	}
	lua_pushboolean(lua, b);
	return(1);
}

int l_artifact_move(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2) or not lua_isstring(lua, 3)) {
		lua_arg_error("artifact_move(inventory_id, artifact_id, destination_inventory_id, [quantity])");
		return(0);
	}

	Uuid inv_id { lua_tostring(lua, 1) };
	Inventory* inventory = Luawrapper::server->getInventory(inv_id);

	if(inventory == nullptr) {
		warning("Inventory "+inv_id.toString()+" doesn't exist.");
		return(0);
	}

	Uuid art_id { lua_tostring(lua, 2) };

	Uuid destination_id { lua_tostring(lua, 3) };
	Inventory* destination = Luawrapper::server->getInventory(destination_id);

	if(destination == nullptr) {
		warning("Inventory "+destination_id.toString()+" doesn't exist.");
		return(0);
	}

	bool returned;
	if(lua_isnumber(lua, 4)) {
		unsigned int quantity = lua_tointeger(lua, 4);
		returned = inventory->move(*destination, art_id, quantity);
	} else {
		returned = inventory->move(*destination, art_id);
	}
	lua_pushboolean(lua, returned);
	return(1);
}

int l_artifact_getname(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("artifact_getname(inventory_id, artifact_id)");
		lua_pushnil(lua);
		return(1);
	}

	Uuid inv_id { lua_tostring(lua, 1) };
	Inventory* inventory = Luawrapper::server->getInventory(inv_id);

	if(inventory == nullptr) {
		warning("Inventory "+inv_id.toString()+" doesn't exist.");
		lua_pushnil(lua);
		return(1);
	}

	Uuid art_id { lua_tostring(lua, 2) };
	Artifact* artifact = inventory->get(art_id);

	if(artifact == nullptr) {
		warning("Artifact "+art_id.toString()+" doesn't exist.");
		lua_pushnil(lua);
		return(1);
	}

	lua_pushstring(lua, artifact->getName().toString().c_str());
	return(1);
}

int l_artifact_setname(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2) or not lua_isstring(lua, 3)) {
		lua_arg_error("artifact_getname(inventory_id,, artifact_id)");
		return(0);
	}

	Uuid inv_id { lua_tostring(lua, 1) };
	Inventory* inventory = Luawrapper::server->getInventory(inv_id);

	if(inventory == nullptr) {
		warning("Inventory "+inv_id.toString()+" doesn't exist.");
		return(0);
	}

	Uuid art_id { lua_tostring(lua, 2) };
	Artifact* artifact = inventory->get(art_id);

	if(artifact == nullptr) {
		warning("Artifact "+art_id.toString()+" doesn't exist.");
		return(0);
	}

	artifact->setName(Name{ lua_tostring(lua, 3) });
	return(0);
}

int l_artifact_gettype(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("artifact_gettype(inventory_id,, artifact_id)");
		lua_pushnil(lua);
		return(1);
	}

	Uuid inv_id { lua_tostring(lua, 1) };
	Inventory* inventory = Luawrapper::server->getInventory(inv_id);

	if(inventory == nullptr) {
		warning("Inventory "+inv_id.toString()+" doesn't exist.");
		lua_pushnil(lua);
		return(1);
	}

	Uuid art_id { lua_tostring(lua, 2) };
	Artifact* artifact = inventory->get(art_id);

	if(artifact == nullptr) {
		warning("Artifact "+art_id.toString()+" doesn't exist.");
		lua_pushnil(lua);
		return(1);
	}

	lua_pushstring(lua, artifact->getType().c_str());
	return(1);
}

int l_artifact_settype(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2) or not lua_isstring(lua, 3)) {
		lua_arg_error("artifact_gettype(inventory_id,, artifact_id)");
		return(0);
	}

	Uuid inv_id { lua_tostring(lua, 1) };
	Inventory* inventory = Luawrapper::server->getInventory(inv_id);

	if(inventory == nullptr) {
		warning("Inventory "+inv_id.toString()+" doesn't exist.");
		return(0);
	}

	Uuid art_id { lua_tostring(lua, 2) };
	Artifact* artifact = inventory->get(art_id);

	if(artifact == nullptr) {
		warning("Artifact "+art_id.toString()+" doesn't exist.");
		return(0);
	}

	artifact->setType(lua_tostring(lua, 3));
	return(0);
}

int l_artifact_getquantity(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2)) {
		lua_arg_error("artifact_getquantity(inventory_id,, artifact_id)");
		lua_pushnil(lua);
		return(1);
	}

	Uuid inv_id { lua_tostring(lua, 1) };
	Inventory* inventory = Luawrapper::server->getInventory(inv_id);

	if(inventory == nullptr) {
		warning("Inventory "+inv_id.toString()+" doesn't exist.");
		lua_pushnil(lua);
		return(1);
	}

	Uuid art_id { lua_tostring(lua, 2) };
	Artifact* artifact = inventory->get(art_id);

	if(artifact == nullptr) {
		warning("Artifact "+art_id.toString()+" doesn't exist.");
		lua_pushnil(lua);
		return(1);
	}

	lua_pushinteger(lua, artifact->getQuantity());
	return(1);
}

int l_artifact_gettag(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2) or not lua_isstring(lua, 3)) {
		lua_arg_error("artifact_gettag(inventory_id, artifact_id, tag)");
		lua_pushnil(lua);
		return(1);
	}

	Uuid inv_id { lua_tostring(lua, 1) };
	Inventory* inventory = Luawrapper::server->getInventory(inv_id);

	if(inventory == nullptr) {
		warning("Inventory "+inv_id.toString()+" doesn't exist.");
		lua_pushnil(lua);
		return(1);
	}

	Uuid art_id { lua_tostring(lua, 2) };
	Artifact* artifact = inventory->get(art_id);

	if(artifact == nullptr) {
		warning("Artifact "+art_id.toString()+" doesn't exist.");
		lua_pushnil(lua);
		return(1);
	}

	lua_pushstring(lua, artifact->getTag(TagID{ lua_tostring(lua, 3) }).toString().c_str());
	return(1);
}

int l_artifact_settag(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2) or not lua_isstring(lua, 3) or not lua_isstring(lua, 4)) {
		lua_arg_error("artifact_settag(inventory_id, artifact_id, tag, value)");
		return(0);
	}

	Uuid inv_id { lua_tostring(lua, 1) };
	Inventory* inventory = Luawrapper::server->getInventory(inv_id);

	if(inventory == nullptr) {
		warning("Inventory "+inv_id.toString()+" doesn't exist.");
		return(0);
	}

	Uuid art_id { lua_tostring(lua, 2) };
	Artifact* artifact = inventory->get(art_id);

	if(artifact == nullptr) {
		warning("Artifact "+art_id.toString()+" doesn't exist.");
		return(0);
	}

	artifact->setTag(TagID{ lua_tostring(lua, 3) }, TagValue{ lua_tostring(lua, 4) });
	return(0);
}

int l_artifact_deltag(lua_State * lua) {
	if(not lua_isstring(lua, 1) or not lua_isstring(lua, 2) or not lua_isstring(lua, 3)) {
		lua_arg_error("artifact_deltag(inventory_id, artifact_id, tag)");
		return(0);
	}

	Uuid inv_id { lua_tostring(lua, 1) };
	Inventory* inventory = Luawrapper::server->getInventory(inv_id);

	if(inventory == nullptr) {
		warning("Inventory "+inv_id.toString()+" doesn't exist.");
		return(0);
	}

	Uuid art_id { lua_tostring(lua, 2) };
	Artifact* artifact = inventory->get(art_id);

	if(artifact == nullptr) {
		warning("Artifact "+art_id.toString()+" doesn't exist.");
		return(0);
	}

	artifact->delTag(TagID{ lua_tostring(lua, 3) });
	return(0);
}

/* Wraper class */

Luawrapper::Luawrapper(class Server * server) :
	lua_state(luaL_newstate())
{
	Luawrapper::server = server;
	luaL_openlibs(this->lua_state);

	lua_register(this->lua_state, "c_rand", l_c_rand);

	lua_register(this->lua_state, "setverbose", l_setverbose);
	lua_register(this->lua_state, "setnoverbose", l_setnoverbose);
	lua_register(this->lua_state, "isverbose", l_isverbose);
	lua_register(this->lua_state, "info", l_info);
	lua_register(this->lua_state, "warning", l_warning);
	lua_register(this->lua_state, "fatal", l_fatal);

	lua_register(this->lua_state, "halt", l_halt);
	lua_register(this->lua_state, "open", l_open);
	lua_register(this->lua_state, "close", l_close);
	lua_register(this->lua_state, "is_open", l_is_open);
	lua_register(this->lua_state, "get_port", l_get_port);
	lua_register(this->lua_state, "delete_zone", l_delete_zone);
	lua_register(this->lua_state, "add_action", l_add_action);
	lua_register(this->lua_state, "get_action", l_get_action);
	lua_register(this->lua_state, "delete_action", l_delete_action);
	lua_register(this->lua_state, "register_aspect", l_register_aspect);

	lua_register(this->lua_state, "create_timer", l_create_timer);
	lua_register(this->lua_state, "delete_timer", l_delete_timer);
	lua_register(this->lua_state, "timer_getremaining", l_timer_getremaining);
	lua_register(this->lua_state, "timer_setremaining", l_timer_setremaining);
	lua_register(this->lua_state, "timer_triggernow", l_timer_triggernow);

	lua_register(this->lua_state, "new_zone", l_new_zone);
	lua_register(this->lua_state, "assert_zone", l_assert_zone);
	lua_register(this->lua_state, "zone_getname", l_zone_getname);
	lua_register(this->lua_state, "zone_setname", l_zone_setname);
	lua_register(this->lua_state, "zone_getwidth", l_zone_getwidth);
	lua_register(this->lua_state, "zone_getheight", l_zone_getheight);
	lua_register(this->lua_state, "zone_event", l_zone_event);

	lua_register(this->lua_state, "place_getaspect", l_place_getaspect);
	lua_register(this->lua_state, "place_setaspect", l_place_setaspect); // Automatically set aspect's default passability.
	lua_register(this->lua_state, "place_ispassable", l_place_ispassable);
	lua_register(this->lua_state, "place_setpassable", l_place_setpassable);
	lua_register(this->lua_state, "place_setnotpassable", l_place_setnotpassable);
	lua_register(this->lua_state, "place_getlandon", l_place_getlandon);
	lua_register(this->lua_state, "place_setlandon", l_place_setlandon);
	lua_register(this->lua_state, "place_resetlandon", l_place_resetlandon);
	lua_register(this->lua_state, "place_gettag", l_place_gettag);
	lua_register(this->lua_state, "place_settag", l_place_settag);
	lua_register(this->lua_state, "place_deltag", l_place_deltag);

	lua_register(this->lua_state, "delete_character", l_delete_character);
	lua_register(this->lua_state, "assert_character", l_assert_character);
	lua_register(this->lua_state, "character_getname", l_character_getname);
	lua_register(this->lua_state, "character_setname", l_character_setname);
	lua_register(this->lua_state, "character_getaspect", l_character_getaspect);
	lua_register(this->lua_state, "character_setaspect", l_character_setaspect);
	lua_register(this->lua_state, "character_getzone", l_character_getzone);
	lua_register(this->lua_state, "character_getx", l_character_getx);
	lua_register(this->lua_state, "character_gety", l_character_gety);
	lua_register(this->lua_state, "character_setxy", l_character_setxy);
	lua_register(this->lua_state, "character_move", l_character_move);
	lua_register(this->lua_state, "character_changezone", l_character_changezone);
	lua_register(this->lua_state, "character_getwhendeath", l_character_getwhendeath);
	lua_register(this->lua_state, "character_setwhendeath", l_character_setwhendeath);
	lua_register(this->lua_state, "character_delgauge", l_character_delgauge);
	lua_register(this->lua_state, "character_gettag", l_character_gettag);
	lua_register(this->lua_state, "character_settag", l_character_settag);
	lua_register(this->lua_state, "character_deltag", l_character_deltag);

	lua_register(this->lua_state, "character_isghost", l_character_isghost);
	lua_register(this->lua_state, "character_setghost", l_character_setghost);
	lua_register(this->lua_state, "character_message", l_character_message);
	lua_register(this->lua_state, "character_follow", l_character_follow);
	lua_register(this->lua_state, "character_hint", l_character_hint);

	lua_register(this->lua_state, "new_gauge", l_new_gauge);
	lua_register(this->lua_state, "assert_gauge", l_assert_gauge);
	lua_register(this->lua_state, "gauge_getname", l_gauge_getname);
	lua_register(this->lua_state, "gauge_setname", l_gauge_setname);
	lua_register(this->lua_state, "gauge_getval", l_gauge_getval);
	lua_register(this->lua_state, "gauge_setval", l_gauge_setval);
	lua_register(this->lua_state, "gauge_increase", l_gauge_increase);
	lua_register(this->lua_state, "gauge_decrease", l_gauge_decrease);
	lua_register(this->lua_state, "gauge_getmax", l_gauge_getmax);
	lua_register(this->lua_state, "gauge_setmax", l_gauge_setmax);
	lua_register(this->lua_state, "gauge_getwhenfull", l_gauge_getwhenfull);
	lua_register(this->lua_state, "gauge_setwhenfull", l_gauge_setwhenfull);
	lua_register(this->lua_state, "gauge_resetwhenfull", l_gauge_resetwhenfull);
	lua_register(this->lua_state, "gauge_getwhenempty", l_gauge_getwhenempty);
	lua_register(this->lua_state, "gauge_setwhenempty", l_gauge_setwhenempty);
	lua_register(this->lua_state, "gauge_resetwhenempty", l_gauge_resetwhenempty);
	lua_register(this->lua_state, "gauge_isvisible", l_gauge_isvisible);
	lua_register(this->lua_state, "gauge_setvisible", l_gauge_setvisible);

	lua_register(this->lua_state, "create_inventory", l_create_inventory);
	lua_register(this->lua_state, "delete_inventory", l_delete_inventory);
	lua_register(this->lua_state, "inventory_size", l_inventory_size);
	lua_register(this->lua_state, "inventory_resize", l_inventory_resize);
	lua_register(this->lua_state, "inventory_total", l_inventory_total);
	lua_register(this->lua_state, "inventory_available", l_inventory_available);
	lua_register(this->lua_state, "inventory_get", l_inventory_get);
	lua_register(this->lua_state, "inventory_get_all", l_inventory_get_all);

	lua_register(this->lua_state, "create_artifact", l_create_artifact);
	lua_register(this->lua_state, "delete_artifact", l_delete_artifact);
	lua_register(this->lua_state, "artifact_move", l_artifact_move);
	lua_register(this->lua_state, "artifact_getname", l_artifact_getname);
	lua_register(this->lua_state, "artifact_setname", l_artifact_setname);
	lua_register(this->lua_state, "artifact_gettype", l_artifact_gettype);
	lua_register(this->lua_state, "artifact_settype", l_artifact_settype);
	lua_register(this->lua_state, "artifact_getquantity", l_artifact_getquantity);
	lua_register(this->lua_state, "artifact_gettag", l_artifact_gettag);
	lua_register(this->lua_state, "artifact_settag", l_artifact_settag);
	lua_register(this->lua_state, "artifact_deltag", l_artifact_deltag);

	this->executeFile(LUA_INIT_SCRIPT);
}

Luawrapper::~Luawrapper() {
	lua_close(this->lua_state);
}

void Luawrapper::executeFile(std::string filename, class Character * character, std::string arg) {
	if(character) {
		lua_pushstring(this->lua_state, character->getId().toString().c_str());
	} else {
		lua_pushnil(this->lua_state);
	}
	lua_setglobal(this->lua_state, "Character");

	if(arg == "") {
		lua_pushnil(this->lua_state);
	} else {
		lua_pushstring(this->lua_state, arg.c_str());
	}
	lua_setglobal(this->lua_state, "Arg");

	luaL_dofile(this->lua_state, filename.c_str());
}

void Luawrapper::executeCode(std::string code, class Character * character, std::string arg) {
	if(character) {
		lua_pushstring(this->lua_state, character->getId().toString().c_str());
	} else {
		lua_pushnil(this->lua_state);
	}
	lua_setglobal(this->lua_state, "Character");

	if(arg == "") {
		lua_pushnil(this->lua_state);
	} else {
		lua_pushstring(this->lua_state, arg.c_str());
	}
	lua_setglobal(this->lua_state, "Arg");

	luaL_dostring(this->lua_state, code.c_str());
}

void Luawrapper::spawnScript(class Character * character) {
	this->executeFile(LUA_SPAWN_SCRIPT, character);
}
