#!/usr/bin/lua
if player_getaspect(Player) == string.byte('@')
then player_setaspect(Player, string.byte('T'))
else player_setaspect(Player, string.byte('@'))
end
