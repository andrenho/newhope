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
-- Create object in physics world
--
function Person:physics_create()
  error('This function should have been replaced by a C function.')
end

-- 
-- Define a target to the person walk towards
function Person:set_target()
  error('This function should have been replaced by a C function.')
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
  self.body = 0
  return self
end


function Person:__tostring()
  return '[Person x:' .. self.x .. ' y:' .. self.y .. ']'
end


return Person

-- vim: ts=2:sw=2:sts=2:expandtab
