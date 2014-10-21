#!/usr/bin/lua
if tile_getid(screen_gettile(player_getscreen(Player), player_getx(Player), player_gety(Player))) == "floor drawn" then
	screen_settile(player_getscreen(Player), player_getx(Player), player_gety(Player), server_gettile(Server, "floor"))
end
