local Person = {}
Person.__index = Person
Person.counter = 0


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
  return self.__id
end


--
-- One frame of the game. Will move if speed != 0.
--
function Person:step()
  if self.__incommunicable > 0 then 
    self.__incommunicable = self.__incommunicable - 1 
  end

  if self.__speed ~= 0 then 
    self:__move() 
  end

  return self
end


--
-- Return if the unit can be interacted with. This is used mainly to avoid 
-- that the unit comunicates again with the same person just by "bumping" into it.
--
function Person:can_talk()
  return (self.__incommunicable == 0)
end

--
-- Return the unit polygon.
-- 
function Person:polygon()
  local p1 = geo.Point:new(self.x - 0.5, self.y - 0.5)
  local p2 = geo.Point:new(self.x + 0.5, self.y - 0.5)
  local p3 = geo.Point:new(self.x + 0.5, self.y + 0.5)
  local p4 = geo.Point:new(self.x - 0.5, self.y + 0.5)
  return geo.Polygon:new {
    geo.Segment:new(p1, p2),
    geo.Segment:new(p2, p3),
    geo.Segment:new(p3, p4),
    geo.Segment:new(p4, p1)
  }
end


----------------------
-- ABSTRACT METHODS --
----------------------


function Person:__respond(person, message, parameters)
  ABSTRACT()
end

function Person:type()
  ABSTRACT()
end


-------------
-- PRIVATE --
-------------


-- 
-- Talk back to a player
--
function Person:__talk(person, message, parameters)
  self.__incommunicable = 30
  local mesg, par = self:__respond(person, message, parameters)
  callback.message(mesg, msg.MSGBOX, {}, 0, person:id())
  return mesg, par
end

-- 
-- Initialize unit (called by implementers)
--
function Person:__init(x, y)
  self.x = x
  self.y = y
  self.image = 1
  self.direction = 0
  self.__speed = 0
  self.__id = Person.counter
  self.__incommunicable = 0
  Person.counter = Person.counter + 1
  return self
end


-- 
-- Move the person.
--
function Person:__move()
  -- convert direction to radians
  local dir_rad = self.direction * math.pi / 180.0

  -- discover future positon
  local step_size = 0.08 * self.__speed
  local fx = self.x + step_size * math.cos(dir_rad)
  local fy = self.y - step_size * math.sin(dir_rad)

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

  -- check if there's a car on the way
  if self:__intersect_car(fx, fy) then
    return false
    -- TODO - enter in the car
  end

  -- ok, can move
  return true
end


-- 
-- Check if the person intersects with a car
--
function Person:__intersect_car(fx, fy)
  local p1 = geo.Point:new(fx - 0.5, fy - 0.5)
  local p2 = geo.Point:new(fx + 0.5, fy - 0.5)
  local p3 = geo.Point:new(fx + 0.5, fy + 0.5)
  local p4 = geo.Point:new(fx - 0.5, fy + 0.5)
  local mypoly = geo.Polygon:new {
    geo.Segment:new(p1, p2),
    geo.Segment:new(p2, p3),
    geo.Segment:new(p3, p4),
    geo.Segment:new(p4, p1)
  }
  for _,car in ipairs(world:cars_in_area(fx-20, fy-20, fx+20, fy+20)) do
    if car:polygon():intersect(mypoly) then 
      return car
    end
  end
  return nil
end


function Person:__tostring()
  return '[Person x:' .. self.x .. ' y:' .. self.y .. ']'
end


return Person

-- vim: ts=2:sw=2:sts=2:expandtab
