local Person = setmetatable({}, DynamicObject)
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
  -- if it's in a car, do nothing
  if self.in_car then
    return
  end

  -- decrease 'incommunicable' by 1 (avoid repeatedly bumping and talking)
  if self.__incommunicable > 0 then 
    self.__incommunicable = self.__incommunicable - 1 
  end

  -- move
  callbacks.set_velocity(self, self.__dir_vect.x * self.__speed, self.__dir_vect.y * self.__speed)

  return self
end


-- 
-- Set direction
-- 
function Person:set_direction_vector(x, y)
  self.__dir_vect.x = x
  self.__dir_vect.y = y
end


--
-- Return if the unit can be interacted with. This is used mainly to avoid 
-- that the unit comunicates again with the same person just by "bumping" into it.
--
function Person:can_talk()
  return (self.__incommunicable == 0)
end


-- 
-- Get ownership of a car
--
function Person:acquire_car(car)
  car.owner = self
  self.car = car
end

--
-- Exit car
-- 
function Person:exit_car()
  local fx = self.x + funct.max{self.car.attrib.w/2, self.car.attrib.h/2} + 0.5
  while not world:tile_walkable(fx, self.y) do 
    fx = fx + 1 
    if fx > 5 then return false end
  end
  self.in_car = false
  self.x = fx
  return true
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
  DynamicObject.__init(self, x, y, 1, 0.5, 75, 0, 9)
  self.__dir_vect = { x = 0, y = 0 }
  self.image = 1
  self.__speed = 10
  self.__id = Person.counter
  self.__incommunicable = 0
  self.car = nil
  self.in_car = false
  Person.counter = Person.counter + 1
  return self
end


-- 
-- Enter in a car
--
function Person:__enter_car()
  assert(self.car)
  self.in_car = true
  self.x = self.car.x
  self.y = self.car.y
  self.direction = self.car:direction()
end


function Person:__tostring()
  return '[Person x:' .. self.x .. ' y:' .. self.y .. ']'
end


return Person

-- vim: ts=2:sw=2:sts=2:expandtab
