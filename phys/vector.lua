local Vector = {}
Vector.__index = Vector

function Vector:new_from_dirmag(magnitude, direction)
  local self = setmetatable({}, Vector)
  self.magnitude, self.direction = magnitude, direction
  return self
end

function Vector:new_from_xy(x, y)
  local magnitude = math.sqrt((x*x) + (y*y))
  local direction = math.atan2(y, x)
  return geo.Vector:new(magnitude, direction)
end

function Vector:terminal_point(initial_point)
  if not initial_point then initial_point = geo.Point:new(0, 0) end
  return geo.Point:new(initial_point.x + self.magnitude * math.cos(self.direction), 
                       initial_point.y - self.magnitude * math.sin(self.direction))
end

function Vector:__add(other)
  local p1 = self:terminal_point()
  local p2 = other:terminal_point()
  return geo.Vector:new_from_xy(p1.x+p2.x, p1.y+p2.y)
end

function Vector:__eq(other)
  return self.magnitude == other.magnitude and self.direction == other.direction
end

function Vector:type()
  return 'Vector'
end

-------------
-- PRIVATE --
-------------

function Vector:__tostring()
  return ('[Vector mag:%0.2f dir:%0.2f]'):format(self.magnitude, self.direction)
end

return Vector

-- vim: ts=2:sw=2:sts=2:expandtab
