# Lua API Reference

# Actor

## ChangeAnimation


```lua
(method) Actor:ChangeAnimation(dir: Direction)
```

 Change the Actor's current animation and play it.

## GetPosition


```lua
(method) Actor:GetPosition()
  -> Vector2
```

 Get the Actor's position

## GetTilePosition


```lua
(method) Actor:GetTilePosition()
```

 Get this Actor's tile position.

## MoveByVelocity


```lua
(method) Actor:MoveByVelocity(velocity: Vector2)
```

 Move the Actor using a velocity vector.

## PlayAnimation


```lua
(method) Actor:PlayAnimation(dir: Direction)
```

 Temporarily play an animation.

## SetPosition


```lua
(method) Actor:SetPosition(newPosition: Vector2)
```

 Set this Actor's position.

## SetTilePosition


```lua
(method) Actor:SetTilePosition(newPosition: Vector2, tileSize: Vector2)
```

 Set Actor position relative to a tile in the world.


---

# ActorContainer

## Exists


```lua
(method) ActorContainer:Exists(roomName: string)
  -> boolean
```

 Whether an Actor with such a name exists in the current room.

## Get


```lua
(method) ActorContainer:Get(roomName: any)
  -> Actor
```

 Get an Actor by its in-room name.

## Push


```lua
(method) ActorContainer:Push(pos: Vector2, typeName: string, roomName: string)
```

 Add a new Actor at this tile position of this type. It will have this in-room name.

## Remove


```lua
(method) ActorContainer:Remove(roomName: string)
```

 Remove an Actor with this in-room name.


---

# CollisionsContainer

## Exists


```lua
(method) CollisionsContainer:Exists(item: Vector2)
```

## Push


```lua
(method) CollisionsContainer:Push(item: Vector2)
```

## Remove


```lua
(method) CollisionsContainer:Remove(item: Vector2)
```


---

# Direction


---

# Game

 The Game namespace is responsible for maanging the state of the program itself


## Close


```lua
function Game.Close()
```

 Closes the game.


---

# GameState

 The StateService is responsible for storing gameplay-related variables
 that make up the state of the game.


## Get


```lua
function GameState.Get(prop: string)
  -> any
```

 Get a property

## Set


```lua
function GameState.Set(prop: string, value: any)
```

 Set the property with the chosen value.


---

# Interactable

## GetPosition


```lua
(method) Interactable:GetPosition()
  -> Vector2
```

 Get the World position of this Interactable

## GetType


```lua
(method) Interactable:GetType()
  -> string
```

 Get this Interactable's type

## IsOnTouch


```lua
(method) Interactable:IsOnTouch()
  -> boolean
```

 Whether this Interactable is set to be interacted with on player collision or not.

## SetOnTouch


```lua
(method) Interactable:SetOnTouch(value: boolean)
```

 Set whether this interactable will be invoked when player touches it.

## SetProp


```lua
(method) Interactable:SetProp(key: any, value: any)
```

 Set an Interactable's property

## SetType


```lua
(method) Interactable:SetType(type: string)
```

 Change the interactable's type.


---

# InteractablesContainer

## Exists


```lua
(method) InteractablesContainer:Exists(pos: Vector2)
```

## GetAt


```lua
(method) InteractablesContainer:GetAt(pos: Vector2)
  -> Interactable
```

## Push


```lua
(method) InteractablesContainer:Push(pos: Vector2, type: string)
```

## Remove


```lua
(method) InteractablesContainer:Remove(pos: Vector2)
```


---

# Interface

 The InterfaceService acts for the User Interface (UI).


## GetCurrentView


```lua
function Interface.GetCurrentView()
```

 Get a reference to the current UI View.

## OpenDialogue


```lua
function Interface.OpenDialogue(dialogue: string)
```

 Open a dialogue with a certain title.

## OpenView


```lua
function Interface.OpenView(title: string)
```

 Open the UI View identified by this title.


---

# InterfaceView

## ChangeFocus


```lua
(method) InterfaceView:ChangeFocus(name: string)
```

 Change focus to another UI element.

## GetEntity


```lua
(method) InterfaceView:GetEntity(name: string)
  -> UIElement
```

 Get an element from the UI View.

## Reset


```lua
(method) InterfaceView:Reset()
```

 Reset the elements to their original positions and sizes.


---

# LuaLS


---

# Player

## GetActor


```lua
(method) Player:GetActor()
  -> Actor
```

 Get the player's actor.

## GetPosition


```lua
(method) Player:GetPosition()
  -> Vector2
```

 Get the player's position

## GetTilePosition


```lua
(method) Player:GetTilePosition()
```

 Get this player's tile position.

## MoveByVelocity


```lua
(method) Player:MoveByVelocity(velocity: Vector2)
```

 Move the Actor using a velocity vector.

## SetPosition


```lua
(method) Player:SetPosition(newPosition: Vector2)
```

 Set this player's position.

## SetTilePosition


```lua
(method) Player:SetTilePosition(newPosition: Vector2)
```

 Set the player's tile position.


---

# Prop

## GetInteractable


```lua
(method) Prop:GetInteractable()
  -> Interactable
```

 Get the Interactable of this Prop.

## GetPosition


```lua
(method) Prop:GetPosition()
  -> Vector2
```

 Get the absolute world position of this Prop.

## GetTilePosition


```lua
(method) Prop:GetTilePosition()
  -> Vector2
```

 Get the tile position of this Prop.

## SetPosition


```lua
(method) Prop:SetPosition(pos: Vector2)
```

 Set the absolute world position of this Prop.

## SetTilePosition


```lua
(method) Prop:SetTilePosition(pos: Vector2)
```

 Set the tile position of this Prop.


---

# PropsContainer

## Exists


```lua
(method) PropsContainer:Exists(pos: Vector2)
```

## GetAt


```lua
(method) PropsContainer:GetAt(pos: Vector2)
  -> Prop
```

## Push


```lua
(method) PropsContainer:Push(pos: Vector2, type: string)
```

## Remove


```lua
(method) PropsContainer:Remove(pos: Vector2)
```


---

# Room

## GetActors


```lua
(method) Room:GetActors()
  -> ActorContainer
```

 Get a refernece to the collection of Actors.

## GetCollisions


```lua
(method) Room:GetCollisions()
  -> CollisionsContainer
```

 Get a reference to the CollisionsContainer of this Room.

## GetInteractables


```lua
(method) Room:GetInteractables()
  -> InteractablesContainer
```

 Get a reference to the InteractablesContainer of this Room.

## GetPlayer


```lua
(method) Room:GetPlayer()
  -> Player
```

 Get a reference to the Player object.

## GetProps


```lua
(method) Room:GetProps()
  -> PropsContainer
```

 Get a reference to the PropsContainer of this Room.

## GetStartTile


```lua
(method) Room:GetStartTile()
  -> Vector2
```

 Set the start tile position.

## GetTileMap


```lua
(method) Room:GetTileMap()
  -> TileMap
```

 Get a pointer to this Room's TileMap.


---

# Sounds

 The SoundService is responsible for playing sounds and managing the current background music


## LoadMusic


```lua
function Sounds.LoadMusic(music: string)
```

 Load the music with the specified id.

## PlayMusic


```lua
function Sounds.PlayMusic()
```

 Play the loaded music.

## PlaySound


```lua
function Sounds.PlaySound(sound: string)
```

 Play the sound with the specified id.


---

# TileMap

## GetWorldSizeInTiles


```lua
(method) TileMap:GetWorldSizeInTiles()
  -> Vector2
```

 Get the size of this TileMap in tiles.

## SetEmptyTile


```lua
(method) TileMap:SetEmptyTile(worldPos: Vector2)
```

 Set the tile at the specified world position to be empty (not drawn).

## SetTile


```lua
(method) TileMap:SetTile(worldPos: Vector2, atlasPos: Vector2)
```

 Set the tile at the specified world position to be a tile at the
 specified atlas position from the TileSet


---

# TweenType


---

# UIElement

## Tween


```lua
(method) UIElement:Tween(property: string, value: any, duration: number, tweenType: TweenType)
```

 Tween this element.


---

# Vector2

## new


```lua
function Vector2.new(x: number, y: number)
```

 Construct a new `Vector2`

## x


```lua
number
```

The x coordinate

## y


```lua
number
```

The y coordinate


---

# World

 The WorldService is responsible for containing and drawing the Room.


## GetPlayer


```lua
function World.GetPlayer()
  -> Player
```

 Get the Player.

## GetRoom


```lua
function World.GetRoom()
  -> Room
```

 Get the current Room

## SetRoom


```lua
function World.SetRoom(room: string)
```

 Set the current room using a room title in the game.
