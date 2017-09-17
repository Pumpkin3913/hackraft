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

create_timer(duration, script) -> timer_id
delete_timer(timer_id)
timer_getremaining(timer_id) -> int | 0
timer_setremaining(timer_id, val)
timer_triggernow(timer_id)

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

delete_character(character_id)
assert_character(character_id) -> bool | nil
character_spawn(character_id, zone_id, x, y)
character_getname(character_id) -> string | nil
character_setname(character_id, name)
character_getaspect(character_id) -> int | nil
character_setaspect(character_id, aspect)
character_getzone(character_id) -> string | nil
character_getx(character_id) -> int | nil
character_gety(character_id) -> int | nil
character_setxy(character_id, x, y)
character_move(character_id, x_shift, y_shift)
character_changezone(character_id, zone_id, x, y)
character_getwhendeath(character_id) -> string | nil
character_setwhendeath(character_id, script)
-- character_list_gauges(character_id)
character_delgauge(character_id, gauge_id)
character_gettag(character_id, tag_id) -> string | tag
character_settag(character_id, tag_id, value)
character_deltag(character_id, tag_id)
character_isghost(character_id) -> bool | nil
character_setghost(character_id, bool)
character_message(character_id, message)
character_follow(character_id, target_character_id)
character_hint(character_id, aspect, hint)

new_gauge(character_id, gauge_id, val, max, aspectFull, aspectEmpty, [, visible])
assert_gauge(character_id, gauge_id) -> bool | nil
gauge_getname(character_id, gauge_id) -> string | nil
gauge_setname(character_id, gauge_id, name)
gauge_getval(character_id, gauge_id) -> int | nil
gauge_setval(character_id, gauge_id, val)
gauge_increase(character_id, gauge_id, val)
gauge_decrease(character_id, gauge_id, val)
gauge_getmax(character_id, gauge_id) -> int | nil
gauge_setmax(character_id, gauge_id, max)
gauge_getwhenfull(character_id, gauge_id) -> string | nil
gauge_setwhenfull(character_id, gauge_id, script)
gauge_resetwhenfull(character_id, gauge_id)
gauge_getwhenempty(character_id, gauge_id) -> string | nil
gauge_setwhenempty(character_id, gauge_id, script)
gauge_resetwhenempty(character_id, gauge_id)
gauge_isvisible(character_id, gauge_id) -> bool | nil
gauge_setvisible(character_id, gauge_id, bool)

create_inventory(size, [type]) -> inventory_id
delete_inventory(inventory_id)
inventory_size(inventory_id) -> int
inventory_resize(inventory_id, size)
inventory_total(inventory_id) -> int
inventory_available(inventory_id) -> int
inventory_get(inventory_id, item_name) -> table of { artifact_id }
inventory_get_all(inventory_id) -> table of { artifact_id }

create_artifact(inventory_id, name, type, [quantity] ) -> artifact_id | nil
delete_artifact(inventory_id, artifact_id, [quantity] ) -> bool
artifact_move(inventory_id, artifact_id, destination_inventory_id, [quantity]) -> bool
artifact_getname(inventory_id, artifact_id) -> string
artifact_setname(inventory_id, artifact_id, name)
artifact_gettype(inventory_id, artifact_id) -> string
artifact_settype(inventory_id, artifact_id, type)
artifact_getquantity(inventory_id, artifact_id) -> int
artifact_gettag(inventory_id, artifact_id, tag_id) -> string
artifact_settag(inventory_id, artifact_id, tag_id, value)
artifact_deltag(inventory_id, artifact_id, tag_id)
