#!/usr/bin/lua
if player_gettag(Player, "damned") == "false" then
	player_settag(Player, "damned", "true")
	local graveyard = server_getscreen(Server, "graveyard")
	local x = screen_getwidth(graveyard)/2
	local y = screen_getheight(graveyard)/2
	player_changescreen(Player, graveyard, x, y)
end
