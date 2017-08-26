c_rand(max) -> int | nil
setverbose()
setnoverbose()
isverbose() -> bool
info(message)
warning(message)
fatal(message)

halt()
open(port)
close()
is_open() -> bool
get_port() -> int
delete_zone(zone_id)

add_action(trigger, script)
get_action(trigger) -> string | nil
delete_action(trigger)
-- list_actions()
register_aspect(string, int [, passable])

new_zone(id, name, width, height, tile_id)
assert_zone(zone_id) -> bool
zone_getname(zone_id) -> string | nil
zone_setname(zone_id, name)
zone_getwidth(zone_id) -> int | nil
zone_getheight(zone_id) -> int | nil
zone_event(zone_id, message)

place_getaspect(zone_id, x, y)
place_setaspect(zone_id, x, y, aspect) // Automatically set aspect's default passability.
place_ispassable(zone_id, x, y)
place_setpassable(zone_id, x, y)
place_setnotpassable(zone_id, x, y)
place_getlandon(zone_id, x, y)
place_setlandon(zone_id, x, y, script)
place_resetlandon(zone_id, x, y)
place_gettag(zone_id, x, y, tag_id)
place_settag(zone_id, x, y, tag_id, value)
place_deltag(zone_id, x, y, tag_id)

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
player_getwhendeath(player_id) -> string | nil
player_setwhendeath(player_id, script)
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
gauge_getwhenfull(player_id, gauge_id) -> string | nil
gauge_setwhenfull(player_id, gauge_id, script)
gauge_resetwhenfull(player_id, gauge_id)
gauge_getwhenempty(player_id, gauge_id) -> string | nil
gauge_setwhenempty(player_id, gauge_id, script)
gauge_resetwhenempty(player_id, gauge_id)
gauge_isvisible(player_id, gauge_id) -> bool | nil
gauge_setvisible(player_id, gauge_id, bool)

