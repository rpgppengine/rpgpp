--- @class Vector2
--- @field x number The x coordinate
--- @field y number The y coordinate
local Vector2 = {}

--- Construct a new `Vector2`
--- @param x number
--- @param y number
function Vector2.new(x, y) end

--- @enum Direction
local Direction = {
    DOWN_IDLE = 0,
    DOWN = 1,
    UP_IDLE = 2,
    UP = 3,
    LEFT_IDLE = 4,
    LEFT = 5,
    RIGHT_IDLE = 6,
    RIGHT = 7
}

--- @class Actor
local Actor = {}

--- Get the Actor's position
--- @return Vector2
function Actor:GetPosition() end

--- Set this Actor's position.
--- @param newPosition Vector2
function Actor:SetPosition(newPosition) end

--- Get this Actor's tile position.
function Actor:GetTilePosition() end

--- Set Actor position relative to a tile in the world.
--- @param newPosition Vector2
--- @param tileSize Vector2
function Actor:SetTilePosition(newPosition, tileSize) end

--- Move the Actor using a velocity vector.
--- @param velocity Vector2
function Actor:MoveByVelocity(velocity) end

--- Change the Actor's current animation and play it.
--- @param dir Direction
function Actor:ChangeAnimation(dir) end

--- Temporarily play an animation.
--- @param dir Direction
function Actor:PlayAnimation(dir) end

--- @class Player
local Player = {}

--- Get the player's position
--- @return Vector2
function Player:GetPosition() end

--- Set this player's position.
--- @param newPosition Vector2
function Player:SetPosition(newPosition) end

--- Get this player's tile position.
function Player:GetTilePosition() end

--- Set the player's tile position.
--- @param newPosition Vector2
function Player:SetTilePosition(newPosition) end

--- Move the Actor using a velocity vector.
--- @param velocity Vector2
function Player:MoveByVelocity(velocity) end

--- Get the player's actor.
--- @return Actor
function Player:GetActor() end

--- @class TileMap
local TileMap = {}

--- Set the tile at the specified world position to be a tile at the
--- specified atlas position from the TileSet
--- @param worldPos Vector2
--- @param atlasPos Vector2
function TileMap:SetTile(worldPos, atlasPos) end

--- Set the tile at the specified world position to be empty (not drawn).
--- @param worldPos Vector2
function TileMap:SetEmptyTile(worldPos) end

--- Get the size of this TileMap in tiles.
--- @return Vector2
function TileMap:GetWorldSizeInTiles() end

--- @class Interactable
local Interactable = {}

--- Set an Interactable's property
--- @param key any
--- @param value any
function Interactable:SetProp(key, value) end

--- Whether this Interactable is set to be interacted with on player collision or not.
--- @return boolean
function Interactable:IsOnTouch() end

--- Set whether this interactable will be invoked when player touches it.
--- @param value boolean
function Interactable:SetOnTouch(value) end

--- Get the World position of this Interactable
--- @return Vector2
function Interactable:GetPosition() end

--- Get this Interactable's type
--- @return string
function Interactable:GetType() end

--- Change the interactable's type.
--- @param type string
function Interactable:SetType(type) end

--- @class Prop
local Prop = {}

--- Get the absolute world position of this Prop.
--- @return Vector2
function Prop:GetPosition() end

--- Set the absolute world position of this Prop.
--- @param pos Vector2
function Prop:SetPosition(pos) end

--- Get the tile position of this Prop.
--- @return Vector2
function Prop:GetTilePosition() end

--- Set the tile position of this Prop.
--- @param pos Vector2
function Prop:SetTilePosition(pos) end

--- Get the Interactable of this Prop.
--- @return Interactable
function Prop:GetInteractable() end

--- @class CollisionsContainer
local CollisionsContainer = {}

--- @param item Vector2
function CollisionsContainer:Push(item) end

--- @param item Vector2
function CollisionsContainer:Remove(item) end

--- @param item Vector2
function CollisionsContainer:Exists(item) end

--- @class InteractablesContainer
local InteractablesContainer = {}

--- @param pos Vector2
--- @param type string
function InteractablesContainer:Push(pos, type) end

--- @param pos Vector2
function InteractablesContainer:Remove(pos) end

--- @param pos Vector2
function InteractablesContainer:Exists(pos) end

--- @param pos Vector2
--- @return Interactable
function InteractablesContainer:GetAt(pos) end

--- @class PropsContainer
local PropsContainer = {}

--- @param pos Vector2
--- @param type string
function PropsContainer:Push(pos, type) end

--- @param pos Vector2
function PropsContainer:Remove(pos) end

--- @param pos Vector2
function PropsContainer:Exists(pos) end

--- @param pos Vector2
--- @return Prop
function PropsContainer:GetAt(pos) end

--- @class ActorContainer
local ActorContainer = {}

--- Add a new Actor at this tile position of this type. It will have this in-room name.
--- @param pos Vector2
--- @param typeName string
--- @param roomName string
function ActorContainer:Push(pos, typeName, roomName) end

--- Remove an Actor with this in-room name.
--- @param roomName string
function ActorContainer:Remove(roomName) end

--- Whether an Actor with such a name exists in the current room.
--- @param roomName string
--- @return boolean
function ActorContainer:Exists(roomName) end

--- Get an Actor by its in-room name.
--- @return Actor
function ActorContainer:Get(roomName) end

--- @class Room
local Room = {}

--- Get a reference to the Player object.
--- @return Player
function Room:GetPlayer() end

--- Set the start tile position.
--- @return Vector2
function Room:GetStartTile() end

--- Get a pointer to this Room's TileMap.
--- @return TileMap
function Room:GetTileMap() end

--- Get a reference to the CollisionsContainer of this Room.
--- @return CollisionsContainer
function Room:GetCollisions() end

--- Get a reference to the InteractablesContainer of this Room.
--- @return InteractablesContainer
function Room:GetInteractables() end

--- Get a reference to the PropsContainer of this Room.
--- @return PropsContainer
function Room:GetProps() end

--- Get a refernece to the collection of Actors.
--- @return ActorContainer
function Room:GetActors() end

--- @enum TweenType
local TweenType = {
	Linear = 0,
	InSine = 1,
	OutSine = 2,
	InOutSine = 3,
	InQuad = 4,
	OutQuad = 5,
	InOutQuad = 6,
	InCubic = 7,
	OutCubic = 8,
	InOutCubic = 9,
	InQuart = 10,
	OutQuart = 11,
	InOutQuart = 12,
	InQuint = 13.
	OutQuint = 14,
	InOutQuint = 15,
	InExpo = 16,
	OutExpo = 17,
	InOutExpo = 18,
	InCirc = 19,
	OutCirc = 20,
	InOutCirc = 21,
	InBack = 22,
	OutBack = 23,
	InOutBack = 24,
	InElastic = 25,
	OutElastic = 26,
	InOutElastic = 27,
	InBounce = 28,
	OutBounce = 29,
	InOutBounce = 30
}

--- @class InterfaceView
local InterfaceView = {}

--- Get an element from the UI View.
--- @param name string
--- @return UIElement
function InterfaceView:GetEntity(name) end

--- Change focus to another UI element.
--- @param name string
function InterfaceView:ChangeFocus(name) end

--- Reset the elements to their original positions and sizes.
function InterfaceView:Reset() end

--- @class UIElement
local UIElement = {}

--- Tween this element.
--- @param property string
--- @param value any
--- @param duration number
--- @param tweenType TweenType
function UIElement:Tween(property, value, duration, tweenType) end
