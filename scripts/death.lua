#!/usr/bin/lua
graveyard = server_getscreen(Server, "graveyard")
math.randomseed(os.time())
x = math.random(screen_getwidth(graveyard)-1)
y = math.random(screen_getheight(graveyard)-1)
place = screen_getplace(graveyard, x, y)
place_settile(place, server_gettile(Server, "grave"))
if player_getname(Player) == "nameless" then
	place_setdescription(place, "A grave with no name.")
else
	place_setdescription(place, "\"Here lies "..player_getname(Player)..". RIP.\"")
end
screen_updatetile(graveyard, x, y);
