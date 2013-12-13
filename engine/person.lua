local Person = setmetatable({}, DynamicObject)
Person.__index = Person


--
-- One frame of the game. Will move if speed != 0.
--
function Person:step()
  -- move
  --physics.set_velocity(self, self.__mov_vect.x * self.__speed, self.__mov_vect.y * self.__speed)

  return self
end


-- 
-- Set direction
-- 
function Person:set_movement_vector(x, y)
  self.__mov_vect.x = x
  self.__mov_vect.y = y
end


----------------------
-- ABSTRACT METHODS --
----------------------


function Person:type()
  ABSTRACT()
end


-------------
-- PRIVATE --
-------------


-- 
-- Initialize unit (called by implementers)
--
function Person:__init(x, y)
  DynamicObject.__init(self, x, y, 0.5, 0.5, 70, 0, 8)
  self.physics_model = DynamicObject.MOVEABLE_PERSON
  self.__mov_vect = { x = 0, y = 0 }
  self.__speed = 4
  return self
end


function Person:__tostring()
  return '[Person x:' .. self.x .. ' y:' .. self.y .. ']'
end


return Person

-- vim: ts=2:sw=2:sts=2:expandtab
