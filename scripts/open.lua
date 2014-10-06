#!/usr/bin/lua

function opendoor(screen, x, y)
	local place = screen_getplace(screen, x, y)
	if tile_getid(place_gettile(place)) == "door" then
		place_setaspect(place, string.byte('.'))
		screen_updatetile(screen, x, y)
		place_setcanland(place)
	end
end

x = player_getx(Player)
y = player_gety(Player)
screen = player_getscreen(Player)

opendoor(screen, x+1, y);
opendoor(screen, x-1, y);
opendoor(screen, x, y+1);
opendoor(screen, x, y-1);

