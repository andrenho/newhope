local Vector = {}
Vector.__index = Vector

function Vector:new_mag(magnitude, angle)
  local self = setmetatable({}, Vector)
  self.magnitude, self.angle = magnitude, angle
  self.x = self.magnitude * math.cos(self.angle)
  self.y = self.magnitude * math.sin(self.angle)
  return self
end

function Vector:new_xy(x, y)
  local self = setmetatable({}, Vector)
  self.x, self.y = x, y
  self.magnitude = math.sqrt((x*x) + (y*y))
  self.angle = math.atan2(y, x)
  return self
end

function Vector:__add(other)
  return geo.Vector:new_xy(self.x+other.x, self.y+other.y)
end

function Vector:__eq(other)
  return self.magnitude == other.magnitude and self.angle == other.angle
end

function Vector:type()
  return 'Vector'
end

-------------
-- PRIVATE --
-------------

function Vector:__tostring()
  return ('[Vector mag:%0.2f dir:%0.2f]'):format(self.magnitude, self.angle)
end

return Vector

-- vim: ts=2:sw=2:sts=2:expandtab
