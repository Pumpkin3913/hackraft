#!/usr/bin/lua
player_setaspect(Player, string.byte('@'))
player_setondeath(Player, "scripts/death.lua")
new_gauge(Player, "life", 3, 3)
player_spawn(Player, server_getscreen(Server, "entrance"), 5, 3)
