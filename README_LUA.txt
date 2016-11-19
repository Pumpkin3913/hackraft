c_rand(max) -> int | nil
setverbose()
setnoverbose()
isverbose() -> bool
warning(message)
nonfatal(message)
fatal(message)
info(message)
verbose(message)

halt()
open(port)
close()
is_open() -> bool
get_port() -> int
delete_zone(zone_id)

add_script(command, filename)
get_script(command) -> string | nil
delete_script(command)
-- list_scripts()

new_tile(id, name, aspect [, passable])
assert_tile(tile_id) -> bool | nil
tile_getname(tile_id) -> string | nil
tile_setname(tile_id, name)
tile_getaspect(tile_id) -> int | nil
tile_setaspect(tile_id, aspect)
tile_canland(tile_id) -> bool | nil
tile_setcanland(tile_id, bool)
-- list_tiles()

new_zone(id, name, width, height, tile_id)
assert_zone(zone_id) -> bool
zone_getname(zone_id) -> string | nil
zone_setname(zone_id, name)
zone_getwidth(zone_id) -> int | nil
zone_getheight(zone_id) -> int | nil
zone_gettile(zone_id, x, y) -> string | nil
zone_settile(zone_id, x, y, tile_id)
-- zone_getplayer(zone, id)
zone_getlandon(zone_id, x, y) -> string | nil
zone_setlandon(zone_id, x, y, script)
zone_resetlandon(zone_id, x, y)
zone_gettag(zone_id, x, y, tag_id)
zone_settag(zone_id, x, y, tag_id, value)
zone_deltag(zone_id, x, y, tag_id)
zone_event(zone_id, message)

delete_player(player_id)
assert_player(player_id) -> bool | nil
player_spawn(player_id, zone_id, x, y)
player_getname(player_id) -> string | nil
player_setname(player_id, name)
player_getaspect(player_id) -> int | nil
player_setaspect(player_id, aspect)
player_getzone(player_id) -> string | nil
player_getx(player_id) -> int | nil
player_gety(player_id) -> int | nil
player_setxy(player_id, x, y)
player_move(player_id, x_shift, y_shift)
player_changezone(player_id, zone_id, x, y)
player_getondeath(player_id) -> string | nil
player_setondeath(player_id, script)
-- player_getgauge(player_id, gauge_id)
-- player_list_gauges(player_id)
player_delgauge(player_id, gauge_id)
player_gettag(player_id, tag_id) -> string | tag
player_settag(player_id, tag_id, value)
player_deltag(player_id, tag_id)
player_isghost(player_id) -> bool | nil
player_setghost(player_id, bool)
player_message(player_id, message)
player_follow(player_id, target_player_id)

new_gauge(player_id, gauge_id, val, max, aspectFull, aspectEmpty, [, visible])
assert_gauge(player_id, gauge_id) -> bool | nil
gauge_getname(player_id, gauge_id) -> string | nil
gauge_setname(player_id, gauge_id, name)
gauge_getval(player_id, gauge_id) -> int | nil
gauge_setval(player_id, gauge_id, val)
gauge_increase(player_id, gauge_id, val)
gauge_decrease(player_id, gauge_id, val)
gauge_getmax(player_id, gauge_id) -> int | nil
gauge_setmax(player_id, gauge_id, max)
gauge_getonfull(player_id, gauge_id) -> string | nil
gauge_setonfull(player_id, gauge_id, script)
gauge_resetonfull(player_id, gauge_id)
gauge_getonempty(player_id, gauge_id) -> string | nil
gauge_setonempty(player_id, gauge_id, script)
gauge_resetonempty(player_id, gauge_id)
gauge_isvisible(player_id, gauge_id) -> bool | nil
gauge_setvisible(player_id, gauge_id, bool)

