#!/usr/bin/lua

local x = player_getx(Player)
local y = player_gety(Player)
local screen = player_getscreen(Player)
local doit = true

if Arg == "north" then
	y = y-1
elseif Arg == "south" then
	y = y+1
elseif Arg == "east" then
	x = x+1
elseif Arg == "west" then
	x = x-1
-- elseif Arg == "help" or Arg ~= "" then
elseif not Arg then
	-- look here
elseif Arg ~= "" then
	player_message(Player, "/look [north|south|east|west|]")
	doit = false
end

if doit then
	local descr = place_getdescription(screen_getplace(screen, x, y))
	player_message(Player, descr)
end
