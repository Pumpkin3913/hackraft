#!/usr/bin/lua

function opendoor(screen, x, y)
	local place = screen_getplace(screen, x, y)
	if tile_getid(place_gettile(place)) == "door" then
		place_setaspect(place, string.byte('_'))
		screen_updatetile(screen, x, y)
		place_setcanland(place)
	end
end

local x = player_getx(Player)
local y = player_gety(Player)
local screen = player_getscreen(Player)

if Arg == "north" then
	opendoor(screen, x, y-1);
elseif Arg == "south" then
	opendoor(screen, x, y+1);
elseif Arg == "east" then
	opendoor(screen, x+1, y);
elseif Arg == "west" then
	opendoor(screen, x-1, y);
else
	player_message(Player, "/open [north|south|east|west]")
end

