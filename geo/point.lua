local Point = {}
Point.__index = Point

function Point:new(x, y)
  local self = setmetatable({}, Point)
  self.x, self.y = self.x, self.y
  return self
end

function Point:__eq(other)
  return (self.x == other.x) and (self.y == other.y)
end

function Point:type()
  return Point
end

-------------
-- PRIVATE --
-------------

function Point:__tostring()
  return ('[Point x:%0.2f y:%0.2f]'):format(self.x, self.y)
end

return Point

-- vim: ts=2:sw=2:sts=2:expandtab
