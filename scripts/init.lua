#!/usr/bin/lua

setverbose()

-- Row 1: mosaic
row = 0
server_addtile(Server, new_tile("palette_a", "", "", row+0))
server_addtile(Server, new_tile("palette_b", "", "", row+1))
server_addtile(Server, new_tile("mosaic_a", "Mosaic", "", row+2))
server_addtile(Server, new_tile("mosaic_b", "Mosaic", "", row+3))
server_addtile(Server, new_tile("mosaic_c", "Mosaic", "", row+4))
server_addtile(Server, new_tile("mosaic_broken", "Broken Mosaic", "", row+5))
server_addtile(Server, new_tile("mosaic_white", "White Mosaic", "", row+6))
server_addtile(Server, new_tile("mosaic_black", "Black Mosaic", "", row+7))
-- Row 2: floor 1/3
row = row+8
server_addtile(Server, new_tile("path_toplft", "", "", row+0))
server_addtile(Server, new_tile("path_top", "", "", row+1))
server_addtile(Server, new_tile("path_toprgt", "", "", row+2))
server_addtile(Server, new_tile("path_endtop", "", "", row+3))
server_addtile(Server, new_tile("path_endlft", "", "", row+4))
server_addtile(Server, new_tile("path_horizontal", "", "", row+5))
server_addtile(Server, new_tile("path_endrgt", "", "", row+6))
server_addtile(Server, new_tile("path_lone", "", "", row+7))
-- Row 3: floor 2/3
row = row+8
server_addtile(Server, new_tile("path_lft", "", "", row+0))
server_addtile(Server, new_tile("path", "", "", row+1))
server_addtile(Server, new_tile("path_rgt", "", "", row+2))
server_addtile(Server, new_tile("path_vertical", "", "", row+3))
server_addtile(Server, new_tile("path_rare_a", "", "", row+4))
server_addtile(Server, new_tile("path_rare_b", "", "", row+5))
server_addtile(Server, new_tile("path_rare_c", "", "", row+6))
server_addtile(Server, new_tile("path_rare_d", "", "", row+7))
-- Row 4: floor 3/3
row = row+8
server_addtile(Server, new_tile("path_botlft", "", "", row+0))
server_addtile(Server, new_tile("path_bot", "", "", row+1))
server_addtile(Server, new_tile("path_botrgt", "", "", row+2))
server_addtile(Server, new_tile("path_endbot", "", "", row+3))
server_addtile(Server, new_tile("soil_a", "", "", row+4))
server_addtile(Server, new_tile("soil_b", "", "", row+5))
server_addtile(Server, new_tile("soil_c", "", "", row+6))
server_addtile(Server, new_tile("soil_d", "", "", row+7))
-- Row 5: roof 1/3
row = row+8
server_addtile(Server, new_tile("roof_toplft", "", "", row+0))
server_addtile(Server, new_tile("roof_top", "", "", row+1))
server_addtile(Server, new_tile("roof_toprgt", "", "", row+2))
server_addtile(Server, new_tile("roof_endtop", "", "", row+3))
server_addtile(Server, new_tile("roof_endlft", "", "", row+4))
server_addtile(Server, new_tile("roof_horizontal", "", "", row+5))
server_addtile(Server, new_tile("roof_endrgt", "", "", row+6))
server_addtile(Server, new_tile("roof_lone", "", "", row+7))
-- Row 6: roof 2/3
row = row+8
server_addtile(Server, new_tile("roof_lft", "", "", row+0))
server_addtile(Server, new_tile("roof", "", "", row+1))
server_addtile(Server, new_tile("roof_rgt", "", "", row+2))
server_addtile(Server, new_tile("roof_vertical", "", "", row+3))
server_addtile(Server, new_tile("roof_rare_a", "", "", row+4))
server_addtile(Server, new_tile("roof_rare_b", "", "", row+5))
server_addtile(Server, new_tile("roof_rare_c", "", "", row+6))
server_addtile(Server, new_tile("roof_rare_d", "", "", row+7))
-- Row 7: roof 3/3
row = row+8
server_addtile(Server, new_tile("roof_botlft", "", "", row+0))
server_addtile(Server, new_tile("roof_bot", "", "", row+1))
server_addtile(Server, new_tile("roof_botrgt", "", "", row+2))
server_addtile(Server, new_tile("roof_endbot", "", "", row+3))
server_addtile(Server, new_tile("wall_door_lft", "", "", row+4))
server_addtile(Server, new_tile("wall_door", "", "", row+5))
server_addtile(Server, new_tile("wall_door_rgt", "", "", row+6))
server_addtile(Server, new_tile("wall_door_little", "", "", row+7))
-- Row 8: wall 1/2
row = row+8
server_addtile(Server, new_tile("wall_lft", "", "", row+0))
server_addtile(Server, new_tile("wall", "", "", row+1))
server_addtile(Server, new_tile("wall_rgt", "", "", row+2))
server_addtile(Server, new_tile("pillar", "", "", row+3))
server_addtile(Server, new_tile("wall_alt_a", "", "", row+4))
server_addtile(Server, new_tile("wall_alt_b", "", "", row+5))
server_addtile(Server, new_tile("wall_alt_c", "", "", row+6))
server_addtile(Server, new_tile("wall_window", "", "", row+7))
-- Row 9: wall 2/2
row = row+8
server_addtile(Server, new_tile("wall_botlft", "", "", row+0))
server_addtile(Server, new_tile("wall_bot", "", "", row+1))
server_addtile(Server, new_tile("wall_botrgt", "", "", row+2))
server_addtile(Server, new_tile("pillar_bot", "", "", row+3))
server_addtile(Server, new_tile("wall_bot_alt_a", "", "", row+4))
server_addtile(Server, new_tile("wall_bot_alt_b", "", "", row+5))
server_addtile(Server, new_tile("wall_bot_alt_c", "", "", row+6))
server_addtile(Server, new_tile("wall_bot_window", "", "", row+7))
-- Row 10: block 1x1
row = row+8
server_addtile(Server, new_tile("block_a", "", "", row+0))
server_addtile(Server, new_tile("block_b", "", "", row+1))
server_addtile(Server, new_tile("block_c", "", "", row+2))
server_addtile(Server, new_tile("block_d", "", "", row+3))
server_addtile(Server, new_tile("block_e", "", "", row+4))
server_addtile(Server, new_tile("block_f", "", "", row+5))
server_addtile(Server, new_tile("block_g", "", "", row+6))
server_addtile(Server, new_tile("block_well", "", "", row+7))
-- Row 11: block 2x2 1/2
row = row+8
server_addtile(Server, new_tile("big1_toplft", "", "", row+0))
server_addtile(Server, new_tile("big1_toprgt", "", "", row+1))
server_addtile(Server, new_tile("big2_toplft", "", "", row+2))
server_addtile(Server, new_tile("big2_toprgt", "", "", row+3))
server_addtile(Server, new_tile("big3_toplft", "", "", row+4))
server_addtile(Server, new_tile("big3_toprgt", "", "", row+5))
server_addtile(Server, new_tile("big4_toplft", "", "", row+6))
server_addtile(Server, new_tile("big4_toprgt", "", "", row+7))
-- Row 12: block 2x2 2/2
row = row+8
server_addtile(Server, new_tile("big1_toplft", "", "", row+0))
server_addtile(Server, new_tile("big1_toprgt", "", "", row+1))
server_addtile(Server, new_tile("big2_toplft", "", "", row+2))
server_addtile(Server, new_tile("big2_toprgt", "", "", row+3))
server_addtile(Server, new_tile("big3_toplft", "", "", row+4))
server_addtile(Server, new_tile("big3_toprgt", "", "", row+5))
server_addtile(Server, new_tile("big4_toplft", "", "", row+6))
server_addtile(Server, new_tile("big4_toprgt", "", "", row+7))
-- Row 13: block 2x3 1/3
row = row+8
server_addtile(Server, new_tile("giant1_toplft", "", "", row+0))
server_addtile(Server, new_tile("giant1_toprgt", "", "", row+1))
server_addtile(Server, new_tile("giant2_toplft", "", "", row+2))
server_addtile(Server, new_tile("giant2_toprgt", "", "", row+3))
server_addtile(Server, new_tile("giant3_toplft", "", "", row+4))
server_addtile(Server, new_tile("giant3_toprgt", "", "", row+5))
server_addtile(Server, new_tile("giant4_toplft", "", "", row+6))
server_addtile(Server, new_tile("giant4_toprgt", "", "", row+7))
-- Row 14: block 2x3 2/3
row = row+8
server_addtile(Server, new_tile("giant1_midlft", "", "", row+0))
server_addtile(Server, new_tile("giant1_midrgt", "", "", row+1))
server_addtile(Server, new_tile("giant2_midlft", "", "", row+2))
server_addtile(Server, new_tile("giant2_midrgt", "", "", row+3))
server_addtile(Server, new_tile("giant3_midlft", "", "", row+4))
server_addtile(Server, new_tile("giant3_midrgt", "", "", row+5))
server_addtile(Server, new_tile("giant4_midlft", "", "", row+6))
server_addtile(Server, new_tile("giant4_midrgt", "", "", row+7))
-- Row 15: block 2x3 3/3
row = row+8
server_addtile(Server, new_tile("giant1_botlft", "", "", row+0))
server_addtile(Server, new_tile("giant1_botrgt", "", "", row+1))
server_addtile(Server, new_tile("giant2_botlft", "", "", row+2))
server_addtile(Server, new_tile("giant2_botrgt", "", "", row+3))
server_addtile(Server, new_tile("giant3_botlft", "", "", row+4))
server_addtile(Server, new_tile("giant3_botrgt", "", "", row+5))
server_addtile(Server, new_tile("giant4_botlft", "", "", row+6))
server_addtile(Server, new_tile("giant4_botrgt", "", "", row+7))
-- Row 16: special
row = row+8
server_addtile(Server, new_tile("coffer_common", "", "", row+0))
server_addtile(Server, new_tile("coffer_unusual", "", "", row+1))
server_addtile(Server, new_tile("coffer_rare", "", "", row+2))
server_addtile(Server, new_tile("coffer_unique", "", "", row+3))
server_addtile(Server, new_tile("pressplate_on", "", "", row+4))
server_addtile(Server, new_tile("pressplate_off", "", "", row+5))
server_addtile(Server, new_tile("portal_on", "", "", row+6))
server_addtile(Server, new_tile("portal_off", "", "", row+7))

-- Entrance
screen_entrance = new_screen("Entrance", 11, 11, server_gettile(Server, "path"))
server_addscreen(Server, "entrance", screen_entrance)
place_settile(screen_getplace(screen_entrance, 5, 3), server_gettile(Server, "mosaic_a"))
place_settile(screen_getplace(screen_entrance, 5, 7), server_gettile(Server, "mosaic_b"))
place_setlandon(screen_getplace(screen_entrance, 5,7), "scripts/plop.lua")

-- Test Room
screen_testroom = new_screen("Test Room", 20, 10, server_gettile(Server, "path"))
server_addscreen(Server, "testroom", screen_testroom)
place_settile(screen_getplace(screen_testroom, 3, 3), server_gettile(Server, "mosaic_b"))
place_setlandon(screen_getplace(screen_testroom, 3,3), "scripts/plop.lua")

-- Player's scripts
server_addscript(Server, "dance", "scripts/dance.lua")
server_addscript(Server, "morph", "scripts/morph.lua")
server_addscript(Server, "name", "scripts/name.lua")

server_open(Server, 31337)
verboseinfo("Lua Init OK")

