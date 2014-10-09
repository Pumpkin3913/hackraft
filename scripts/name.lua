#!/usr/bin/lua
if Arg and player_getname(Player) == "noname" then
	player_setname(Player, Arg)
end
