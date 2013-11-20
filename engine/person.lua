local Person = {}
Person.__index = Person

-- abstract methods
function Person:step() error('abstract method') end
function Person:respond_to(person, msg, parameters) error('abstract method') end

function Person:__init(x, y)
  self.x = x
  self.y = y
  self.image = 1
  self.direction = 0
  self.__speed = 0
  return self
end


function Person:turn_to(dir)
  self.direction = dir
  return self
end


function Person:change_speed(speed)
  self.__speed = speed
  return self
end



function Person:id()
  error('Not implemented')
end

-------------
-- PRIVATE --
-------------

function Person:__tostring()
  return '[Person x:' .. self.x .. ' y:' .. self.y .. ']'
end

return Person

-- vim: ts=2:sw=2:sts=2:expandtab
