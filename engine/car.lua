local Car = {}
Car.__index = Car

function Car:new(x, y, model)
  local self = setmetatable({}, Car)
  self.x, self.y = x, y
  self.model = model
  self.direction = 0
  return self
end

function Car:step()
  -- TODO
end

-------------
-- PRIVATE --
-------------

function Car:__tostring()
  return '[Car]'
end

return Car

-- vim: ts=2:sw=2:sts=2:expandtab
