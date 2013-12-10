local Point = {}
Point.__index = Point

function Point:new(x, y)
  assert(type(x) == 'number' and type(y) == 'number')
  local self = setmetatable({}, Point)
  self.x, self.y = x, y
  return self
end

function Point:__eq(other)
  return (self.x == other.x) and (self.y == other.y)
end

function Point:distance(other)
  assert(other:type() == 'Point')
  return math.sqrt(math.pow(self.x-other.x, 2) + math.pow(self.y-other.y, 2))
end

function Point:distance_manhattan(other)
  assert(other:type() == 'Point')
  return self.x - other.x + self.y - other.y
end

function Point:translate(other)
  return geo.Point:new(self.x+other.x, self.y+other.y)
end

function Point:rotate(angle)
  return geo.Point:new(self.x * math.cos(angle) - self.y * math.sin(angle),
                       self.y * math.sin(angle) - self.x * math.cos(angle))
end

function Point:type()
  return 'Point'
end

-------------
-- PRIVATE --
-------------

function Point:__tostring()
  return ('[Point x:%0.2f y:%0.2f]'):format(self.x, self.y)
end

return Point

-- vim: ts=2:sw=2:sts=2:expandtab
