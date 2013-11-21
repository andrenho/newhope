local Person = {}
Person.__index = Person


--
-- Turn person to a different direction.
--
function Person:turn_to(dir)
  self.direction = dir
  return self
end


--
-- Change person speed.
--
function Person:change_speed(speed)
  self.__speed = speed
  return self
end


-- 
-- Return a unique ID for the unit.
--
function Person:id()
  error('Not implemented')
end


--
-- One frame of the game. Will move if speed != 0.
--
function Person:step()
  if self.__speed ~= 0 then 
    self:__move() 
  end
  self:__respond_to_interaction()

  return self
end


----------------------
-- ABSTRACT METHODS --
----------------------


function Person:__respond_to_interaction()
  ABSTRACT()
end


-------------
-- PRIVATE --
-------------


-- 
-- Initialize unit (called by implementers)
--
function Person:__init(x, y)
  self.x = x
  self.y = y
  self.image = 1
  self.direction = 0
  self.__speed = 0
  return self
end


-- 
-- Move the person.
--
function Person:__move()
  -- discover future positon
  local step_size = 0.08 * self.__speed
  local fx = self.x + step_size * math.cos(self.direction)
  local fy = self.y + step_size * math.sin(self.direction)

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
    return true
  end
  return false
end

--
-- Check if the person can move over a tile.
--
function Person:__can_move(fx, fy)
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


function Person:__tostring()
  return '[Person x:' .. self.x .. ' y:' .. self.y .. ']'
end


return Person

-- vim: ts=2:sw=2:sts=2:expandtab
