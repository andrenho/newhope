local Person = {}
Person.__index = Person
Person.__tostring = function() return 'Person' end

function Person:new(x, y)
  local self = setmetatable({ }, Person)
  self.x = x
  self.y = y
  self.image = 0
  self.direction = 90
  self.__speed = 0
  return self
end

function Person:__tostring()
  return '[Person x:' .. self.x .. ' y:' .. self.y .. ']'
end

function Person:turn_to(dir)
  self.direction = dir
end

function Person:change_speed(speed)
  self.__speed = speed
end

function Person:step()
end

return Person

-- vim: ts=2:sw=2:sts=2:expandtab
