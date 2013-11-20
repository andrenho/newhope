local Player = setmetatable({}, Person)
Player.__index = Player

--
-- Create a new hero (player)
--
function Player:new(x, y)
  local self = setmetatable({}, Player)
  self:__init(x, y)
  self.image = 0
  return self
end

--
-- One frame of the game. Will move if speed != 0.
--
function Player:step()
  if self.__speed ~= 0 then 
    self:__move() 
  end

  return self
end


-------------
-- PRIVATE --
-------------

function Player:__tostring()
  return '[Player x:' .. self.x .. ' y:' .. self.y .. ']'
end

--
-- Move the unit.
--
function Player:__move()
  -- discover future positon
  local step_size = 0.08 * self.__speed
  local fx = self.x + step_size * math.cos(self.direction)
  local fy = self.y + step_size * math.sin(self.direction)

  -- check if there's a unit to interact in that position
  local p = world:person_in_position(fx, fy, self)
  if p then
    self:__interact_with(p)
  end

  -- if it can't move diagonally, try turning 45º
  if fx ~= self.x and fy ~= self.y and not self:__can_move(fx, fy) then
    if self:__can_move(fx, self.y) then
      fy = self.y
    elseif self:__can_move(self.x, fy) then
      fx = self.x
    end

  end
  -- move
  if self:__can_move(fx, fy) then
    self.x, self.y = fx, fy
  end
end
 
--
-- Check if the hero can move over a tile.
--
function Player:__can_move(fx, fy)
  -- check if going out of bounds
  if fx < 0.5 or fy < 0.5 or fx > (world.w-0.5) or fy > (world.h-0.5) then
    return false
  end

  -- check if going over a not-walkable tile
  local tiles = {
    [world:unique_tile_id(math.floor(fx-0.5), math.floor(fy-0.5))] = true,
    [world:unique_tile_id(math.floor(fx+0.5), math.floor(fy-0.5))] = true,
    [world:unique_tile_id(math.floor(fx-0.5), math.floor(fy+0.5))] = true,
    [world:unique_tile_id(math.floor(fx+0.5), math.floor(fy+0.5))] = true
  }
  for k,_ in pairs(tiles) do
    if not world:tile_walkable(world:revert_unique_tile(k)) then
      return false
    end
  end

  -- ok, can move
  return true
end


--
-- Interact with another unit
--
Player.S_HELLO = 'Hello!'
function Player:__interact_with(p)
  local message = Player.S_HELLO
  print(self, message)
  local response = p:respond_to(self, message)
  print(p, response)
end


return Player

-- vim: ts=2:sw=2:sts=2:expandtab
