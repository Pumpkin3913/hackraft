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
new_zone("entrance", "Entrance", 11, 11, "floor")
zone_settile("entrance", 5, 3, "portal")
zone_settile("entrance", 5, 7, "portal")
zone_setlandon("entrance", 5, 7, "scripts/plop.lua")

-- Test Room
local testroom_height = 11;
new_zone("testroom", "Test Room", 20, testroom_height, "floor")
zone_settile("testroom", 3, 3, "portal")
zone_setlandon("testroom", 3, 3, "scripts/plop.lua")
for i=0, testroom_height-1, 1 do
	zone_settile("testroom", 7, i, "wall")
end
zone_settile("testroom", 7, testroom_height/2, "door")
zone_settile("testroom", 13, 5, "spikes")
zone_setlandon("testroom", 13, 5, "scripts/spikes.lua")

-- Graveyard
new_zone("graveyard", "Graveyard", 30, 30, "floor")

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

