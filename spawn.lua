#!/usr/bin/lua
player_setaspect(Player, string.byte('@'))
player_setondeath(Player, "scripts/death.lua")
new_gauge(Player, "life", 3, 3, string.byte('o'), string.byte('.'))
gauge_setonempty(Player, "life", "scripts/kill.lua")
new_gauge(Player, "xp", 0, 10, string.byte('#'), string.byte('_'), false)
player_settag(Player, "damned", "false")
player_spawn(Player, "entrance", 5, 3)
