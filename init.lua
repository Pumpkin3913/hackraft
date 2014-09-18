#!/usr/bin/lua

tile_floor = new_tile("floor", "Floor", "An old paved floor.", 46)
tile_portal = new_tile("portal", "Portal", "A magic portal leading elsewere.", 94)
server_addtile(Server, tile_floor)
server_addtile(Server, tile_portal)

screen_entrance = new_screen("Entrance", 11, 11, server_gettile(Server, "floor"))
server_addscreen(Server, "entrance", screen_entrance)
place_settile(screen_getplace(screen_entrance, 5, 5), tile_portal)

server_open(Server, 31337)
print("Init OK") -- XXX
-- verbose_info("Init OK") -- TODO

