local Car = setmetatable({}, Object)
Car.__index = Car

function Car:new(x, y, model)
  local self = setmetatable({}, Car)
  self:__init(x, y)
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

function Car:step()
  self:__calculate_forces()
end

function Car:type()
  return 'Car'
end

function Car:is_car()
  return true
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

function Car:__calculate_forces()
  self:__reset_forces()
  if self.controls.accelerate then
    self:__apply_force(1000, 0, 1)
  end
end

function Car:__apply_force(force, relative_dir, wheel)
  error('This method must be implemented in C.')
end

function Car:__reset_forces()
  error('This method must be implemented in C.')
end

return Car

-- vim: ts=2:sw=2:sts=2:expandtab
