#!/usr/bin/lua
player_setaspect(Player, string.byte('@'))
player_setondeath(Player, "scripts/death.lua")
gauge_setonempty(new_gauge(Player, "life", 3, 3, string.byte('o'), string.byte('.')), "scripts/kill.lua")
new_gauge(Player, "xp", 0, 10, string.byte('#'), string.byte('_'), false)
player_spawn(Player, server_getscreen(Server, "entrance"), 5, 3)
