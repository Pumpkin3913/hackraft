#!/usr/bin/lua
graveyard = server_getscreen(Server, "graveyard")
x = screen_getwidth(graveyard)/2
y = screen_getheight(graveyard)/2
player_changescreen(Player, graveyard, x, y)
