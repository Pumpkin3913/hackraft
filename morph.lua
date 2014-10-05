#!/usr/bin/lua
if Arg == "troll" then
	player_setaspect(Player, string.byte('T'))
else if Arg == "naga" then
	player_setaspect(Player, string.byte('N'))
else
	player_setaspect(Player, string.byte('@'))
end
end
