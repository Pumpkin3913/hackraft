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

if Arg == "north" then
	closedoor(screen, x, y-1);
else if Arg == "south" then
	closedoor(screen, x, y+1);
else if Arg == "east" then
	closedoor(screen, x+1, y);
else if Arg == "west" then
	closedoor(screen, x-1, y);
else
	player_message(Player, "/close [north|south|east|west]")
end end end end


