#!/usr/bin/lua
if tile_getid(screen_gettile(player_getscreen(Player), player_getx(Player), player_gety(Player))) ~= "floor" then
	player_message(Player, "I can't /draw here!")
elseif not Arg then
	player_message(Player, "/draw [piece of chalk]")
else
	local chalk = player_getobject(Player, Arg)
	if object_gettag(chalk, "draw") ~= "true" then
		player_message(Player, "I can't /draw with this!")
	else
		local charges = tonumber(object_gettag(chalk, "charges"))
		if charges <= 0 then
			player_message(Player, "This piece of chalk is too used.")
		else
			screen_settile(player_getscreen(Player), player_getx(Player), player_gety(Player), server_gettile(Server, "floor drawn"))
			charges = charges - 1
			object_settag(chalk, "charges", charges);
		end
	end
end
