#!/usr/bin/lua
player_setaspect(Player, string.byte('@'))
player_spawn(Player, server_getscreen(Server, "entrance"), 5, 3)
player_setondeath(Player, "scripts/death.lua")
