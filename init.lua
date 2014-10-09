#!/usr/bin/lua

setverbose()
verboseinfo("Lua Init...")

tile_floor = new_tile("floor", "Floor", "An old paved floor.", string.byte('.'))
tile_portal = new_tile("portal", "Portal", "A magic portal leading elsewere.", string.byte('^'))
tile_door = new_tile("door", "Door", "A wooden door.", string.byte('#'), false)
tile_grave = new_tile("grave", "Grave", "A stone cross.", string.byte('+'), false)
tile_spikes = new_tile("spikes", "Spikes", "Some iron spikes.", string.byte('"'))
server_addtile(Server, tile_floor)
server_addtile(Server, tile_portal)
server_addtile(Server, tile_door)
server_addtile(Server, tile_grave)
server_addtile(Server, tile_spikes)

-- Entrance
screen_entrance = new_screen("Entrance", 11, 11, tile_floor)
server_addscreen(Server, "entrance", screen_entrance)
place_settile(screen_getplace(screen_entrance, 5, 3), tile_portal)
place_settile(screen_getplace(screen_entrance, 5, 7), tile_portal)
place_setlandon(screen_getplace(screen_entrance, 5,7), "scripts/plop.lua")

-- Test Room
screen_testroom = new_screen("Test Room", 20, 10, tile_floor)
server_addscreen(Server, "testroom", screen_testroom)
place_settile(screen_getplace(screen_testroom, 3, 3), tile_portal)
place_setlandon(screen_getplace(screen_testroom, 3, 3), "scripts/plop.lua")
place_settile(screen_getplace(screen_testroom, 3, 6), tile_door)
place_settile(screen_getplace(screen_testroom, 13, 5), tile_spikes)
place_setlandon(screen_getplace(screen_testroom, 13, 5), "scripts/spikes.lua")

-- Graveyard
screen_graveyard = new_screen("Graveyard", 30, 30, tile_floor)
server_addscreen(Server, "graveyard", screen_graveyard)

-- Player's scripts
server_addscript(Server, "dance", "scripts/dance.lua")
server_addscript(Server, "morph", "scripts/morph.lua")
server_addscript(Server, "name", "scripts/name.lua")
server_addscript(Server, "open", "scripts/open.lua")
server_addscript(Server, "close", "scripts/close.lua")
server_addscript(Server, "damn", "scripts/damn.lua")

server_open(Server, 31337)
verboseinfo("Lua Init OK")

