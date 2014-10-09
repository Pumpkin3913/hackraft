#!/usr/bin/lua
local graveyard = server_getscreen(Server, "graveyard")
local x = c_rand(screen_getwidth(graveyard)-1)
local y = c_rand(screen_getheight(graveyard)-1)
local place = screen_getplace(graveyard, x, y)
local tile
if c_rand(2) == 1 then
	tile = server_gettile(Server, "grave_a")
else
	tile = server_gettile(Server, "grave_b")
end
place_settile(place, tile)
if player_getname(Player) == "nameless" then
	place_setdescription(place, "A grave with no name.")
else
	local dice = c_rand(3)
	if dice == 1 then
		place_setdescription(place, "\"Here lies "..player_getname(Player)..".\"")
	elseif dice == 2 then
		place_setdescription(place, "\"RIP "..player_getname(Player)..".\"")
	elseif dice == 3 then
		place_setdescription(place, "\".:["..player_getname(Player).."]:.\"")
	end
end
screen_updatetile(graveyard, x, y);
