--- The WorldService is responsible for containing and drawing the Room.
---
--- @class World
local World = {}

--- Get the Player.
--- @return Player
function World.GetPlayer() end

--- Set the current room using a room title in the game.
--- @param room string
function World.SetRoom(room) end

--- Get the current Room
--- @return Room
function World.GetRoom() end

return World
