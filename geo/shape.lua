local Shape = {}
Shape.__index = Shape

function Shape:new()
  local self = setmetatable({}, Shape)
  return self
end

function Shape:type()
  return Shape
end

-------------
-- PRIVATE --
-------------

function Shape:__tostring()
  return '[Shape]'
end

return Shape

-- vim: ts=2:sw=2:sts=2:expandtab
