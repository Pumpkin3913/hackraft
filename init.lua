#!/usr/bin/lua

setverbose()
-- verboseinfo("Lua Init...")
verbose("Lua Init...")

new_tile("floor", "old paved floor", string.byte('.'))
new_tile("floor drawn", "old paved floor with chalk glyph", string.byte(';'))
new_tile("portal", "magic portal", string.byte('^'))
new_tile("door", "wooden door, closed", string.byte('|'), false)
new_tile("door_open", "wooden door, opened", string.byte('_'))
new_tile("grave_a", "stone cross grave", string.byte('+'), false)
new_tile("grave_b", "stone slab grave", string.byte('n'), false)
new_tile("wall", "stone brick wall", string.byte('#'), false)
new_tile("spikes", "iron spikes", string.byte('"'))

verbose("Tiles created...")

-- Entrance
new_screen("entrance", "Entrance", 11, 11, "floor")
screen_settile("entrance", 5, 3, "portal")
screen_settile("entrance", 5, 7, "portal")
screen_setlandon("entrance", 5, 7, "scripts/plop.lua")

-- local chalk = new_object("piece of chalk", string.byte('-'))
-- object_settag(chalk, "draw", "true")
-- object_settag(chalk, "charges", "10")
-- screen_addobject(screen_entrance, 8, 7, chalk)
-- screen_addobject(screen_entrance, 2, 2, new_object("gem", string.byte('*')))

-- Test Room
local testroom_height = 11;
new_screen("testroom", "Test Room", 20, testroom_height, "floor")
screen_settile("testroom", 3, 3, "portal")
screen_setlandon("testroom", 3, 3, "scripts/plop.lua")
for i=0, testroom_height-1, 1 do
	screen_settile("testroom", 7, i, "wall")
end
screen_settile("testroom", 7, testroom_height/2, "door")
screen_settile("testroom", 13, 5, "spikes")
screen_setlandon("testroom", 13, 5, "scripts/spikes.lua")

-- Graveyard
new_screen("graveyard", "Graveyard", 30, 30, "floor")

verbose("PLOP")

-- Player's scripts
add_script("dance", "scripts/dance.lua")
add_script("morph", "scripts/morph.lua")
add_script("name", "scripts/name.lua")
add_script("open", "scripts/open.lua")
add_script("close", "scripts/close.lua")
add_script("damn", "scripts/damn.lua")
add_script("look", "scripts/look.lua")
add_script("draw", "scripts/draw.lua")
add_script("wipe", "scripts/wipe.lua")

open(31337)
verbose("Lua Init OK")

