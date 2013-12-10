local Rectangle = {}
Rectangle.__index = Rectangle

function Rectangle:new()
  local self = setmetatable({}, Rectangle)
  return self
end

function Rectangle:type()
  return Rectangle
end

-------------
-- PRIVATE --
-------------

function Rectangle:__tostring()
  return '[Rectangle]'
end

return Rectangle

-- vim: ts=2:sw=2:sts=2:expandtab
