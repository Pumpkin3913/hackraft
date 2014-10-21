#!/usr/bin/lua

setverbose()
verboseinfo("Lua Init...")

server_addtile(Server, new_tile("floor", "old paved floor", string.byte('.')))
server_addtile(Server, new_tile("floor drawn", "old paved floor with chalk glyph", string.byte(';')))
server_addtile(Server, new_tile("portal", "magic portal", string.byte('^')))
server_addtile(Server, new_tile("door", "wooden door, closed", string.byte('|'), false))
server_addtile(Server, new_tile("door_open", "wooden door, opened", string.byte('_')))
server_addtile(Server, new_tile("grave_a", "stone cross grave", string.byte('+'), false))
server_addtile(Server, new_tile("grave_b", "stone slab grave", string.byte('n'), false))
server_addtile(Server, new_tile("wall", "stone brick wall", string.byte('#'), false))
server_addtile(Server, new_tile("spikes", "iron spikes", string.byte('"')))

-- Entrance
local screen_entrance = new_screen("Entrance", 11, 11, server_gettile(Server, "floor"))
server_addscreen(Server, "entrance", screen_entrance)
screen_settile(screen_entrance, 5, 3, server_gettile(Server, "portal"))
screen_settile(screen_entrance, 5, 7, server_gettile(Server, "portal"))
screen_setlandon(screen_entrance, 5, 7, "scripts/plop.lua")
local chalk = new_object("piece of chalk", string.byte('-'))
object_settag(chalk, "draw", "true")
object_settag(chalk, "charges", "10")
screen_addobject(screen_entrance, 8, 7, chalk)
screen_addobject(screen_entrance, 2, 2, new_object("gem", string.byte('*')))

-- Test Room
local testroom_height = 11;
local screen_testroom = new_screen("Test Room", 20, testroom_height, server_gettile(Server, "floor"))
server_addscreen(Server, "testroom", screen_testroom)
screen_settile(screen_testroom, 3, 3, server_gettile(Server, "portal"))
screen_setlandon(screen_testroom, 3, 3, "scripts/plop.lua")
for i=0, testroom_height-1, 1 do
	screen_settile(screen_testroom, 7, i, server_gettile(Server, "wall"))
end
screen_settile(screen_testroom, 7, testroom_height/2, server_gettile(Server, "door"))
screen_settile(screen_testroom, 13, 5, server_gettile(Server, "spikes"))
screen_setlandon(screen_testroom, 13, 5, "scripts/spikes.lua")

-- Graveyard
local screen_graveyard = new_screen("Graveyard", 30, 30, server_gettile(Server, "floor"))
server_addscreen(Server, "graveyard", screen_graveyard)

-- Player's scripts
server_addscript(Server, "dance", "scripts/dance.lua")
server_addscript(Server, "morph", "scripts/morph.lua")
server_addscript(Server, "name", "scripts/name.lua")
server_addscript(Server, "open", "scripts/open.lua")
server_addscript(Server, "close", "scripts/close.lua")
server_addscript(Server, "damn", "scripts/damn.lua")
server_addscript(Server, "look", "scripts/look.lua")
server_addscript(Server, "draw", "scripts/draw.lua")
server_addscript(Server, "wipe", "scripts/wipe.lua")

server_open(Server, 31337)
verboseinfo("Lua Init OK")

