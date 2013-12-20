local Vehicle = setmetatable({}, Object)
Vehicle.__index = Vehicle

function Vehicle:new(x, y, model)
  local self = setmetatable({}, Vehicle)
  self:__init(x, y)
  self.is_vehicle = true
  self.controls = {
    accelerate = false,
    breaking = false,
    left = 0,
    right = 0,
    gear = 1,
  }
  self.angle = math.pi/8
  self.__model = model
  self:__calculate_attributes()
  return self
end

function Vehicle:type()
  return 'Vehicle'
end

-------------
-- PRIVATE --
-------------

function Vehicle:__tostring()
  return '[Vehicle]'
end

function Vehicle:__calculate_attributes()
  self.attrib = table.shallow_copy(self.__model)
end

return Vehicle

-- vim: ts=2:sw=2:sts=2:expandtab
