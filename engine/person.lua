local Person = {}
Person.__index = Person


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


function Person:step()
  error('abstract method')
end


function Person:id()
  error('Not implemented')
end


function Person:interact_with(person)
  error('abstract method')
end


-------------
-- PRIVATE --
-------------

function Person:__tostring()
  return '[Person x:' .. self.x .. ' y:' .. self.y .. ']'
end

return Person

-- vim: ts=2:sw=2:sts=2:expandtab
