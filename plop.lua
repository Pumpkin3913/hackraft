#!/usr/bin/lua
-- info("plop");
-- if Player then screen_event(player_getscreen(Player), "Plop") end

screen_event(player_getscreen(Player), "Plop")

if player_getscreen(Player) == server_getscreen(Server, "testroom")
then player_changescreen(Player, server_getscreen(Server, "entrance"), 5, 7)
else player_changescreen(Player, server_getscreen(Server, "testroom"), 3, 3)
end

