#!/usr/bin/lua

function closedoor(screen, x, y)
	local id = tile_getid(screen_gettile(screen, x, y, place))
	if id == "door_open" then
		screen_settile(screen, x, y, server_gettile(Server, "door"))
	end
end

local x = player_getx(Player)
local y = player_gety(Player)
local screen = player_getscreen(Player)

if Arg == "north" then
	closedoor(screen, x, y-1);
elseif Arg == "south" then
	closedoor(screen, x, y+1);
elseif Arg == "east" then
	closedoor(screen, x+1, y);
elseif Arg == "west" then
	closedoor(screen, x-1, y);
else
	player_message(Player, "/close [north|south|east|west]")
end

