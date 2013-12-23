local Vehicle = setmetatable({}, Object)
Vehicle.__index = Vehicle

function Vehicle:new(x, y, model)
  self = self:__init(x, y)
  self.is_vehicle = true
  self.controls = {
    accelerate = false,
    breaking = false,
    steering = 0,
    gear = 1,
  }
  self.angle = math.pi/8
  self.__model = model
  self:__calculate_attributes()
  return self
end

function Vehicle:step()
  self:__update_physics()
end

-------------
-- PRIVATE --
-------------

function Vehicle:__calculate_attributes()
  self.attrib = table.shallow_copy(self.__model)
end

function Vehicle:__tostring()
  return '[Vehicle]'
end

function Vehicle:type()
  return 'Vehicle'
end

return Vehicle

-- vim: ts=2:sw=2:sts=2:expandtab
