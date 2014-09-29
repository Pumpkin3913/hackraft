#!/usr/bin/lua

setverbose()

tile_floor = new_tile("floor", "Floor", "An old paved floor.", 46)
tile_portal = new_tile("portal", "Portal", "A magic portal leading elsewere.", 94)
server_addtile(Server, tile_floor)
server_addtile(Server, tile_portal)

-- Entrance
screen_entrance = new_screen("Entrance", 11, 11, server_gettile(Server, "floor"))
server_addscreen(Server, "entrance", screen_entrance)
place_settile(screen_getplace(screen_entrance, 5, 3), tile_portal)
place_setlandon(screen_getplace(screen_entrance, 5,7), "plop.lua")

-- Test Room
screen_testroom = new_screen("Test Room", 20, 10, server_gettile(Server, "floor"))
server_addscreen(Server, "testroom", screen_testroom)
place_settile(screen_getplace(screen_testroom, 3, 3), tile_portal)
place_setlandon(screen_getplace(screen_testroom, 3,3), "plop.lua")

-- /dance script
server_addscript(Server, "dance", "dance.lua");

server_open(Server, 31337)
verboseinfo("Lua Init OK")

