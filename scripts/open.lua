#!/usr/bin/lua

function opendoor(screen, x, y)
	local id = tile_getid(screen_gettile(screen, x, y, place))
	if id == "door" then
		screen_settile(screen, x, y, server_gettile(Server, "door_open"))
	-- elseif id == "door_open" then
		-- screen_settile(screen, x, y, server_gettile(Server, "door"))
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

