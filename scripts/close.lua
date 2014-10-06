#!/usr/bin/lua

function closedoor(screen, x, y)
	local place = screen_getplace(screen, x, y)
	if tile_getid(place_gettile(place)) == "door" then
		place_resetaspect(place)
		screen_updatetile(screen, x, y)
		place_resetcanland(place)
	end
end

x = player_getx(Player)
y = player_gety(Player)
screen = player_getscreen(Player)

closedoor(screen, x+1, y);
closedoor(screen, x-1, y);
closedoor(screen, x, y+1);
closedoor(screen, x, y-1);

