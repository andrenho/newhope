local Car = setmetatable({}, Object)
Car.__index = Car

function Car:new(x, y, model)
  local self = self or setmetatable({}, Car)
  self.controls = {
    accelerate = false,
    breaking = false,
    left = 0,
    right = 0,
    gear = 1,
  }
  self.angle = 0
  self.__model = model
  self:__calculate_attributes()
  return self
end

function Car:type()
  return 'Car'
end

-------------
-- PRIVATE --
-------------

function Car:__tostring()
  return '[Car]'
end

function Car:__calculate_attributes()
  self.attrib = table.shallow_copy(self.__model)
end

return Car

-- vim: ts=2:sw=2:sts=2:expandtab
