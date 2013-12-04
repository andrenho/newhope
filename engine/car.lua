local Car = {}
Car.__index = Car

function Car:new(x, y, model)
  local self = setmetatable({}, Car)
  self.x, self.y = x, y
  self.model = model
  self.direction = 0
  return self
end

--
-- Return the car polygon
-- 
function Car:polygon()
  local dir_rad = self.direction * math.pi / 180.0
  local sw = self.model.w/2
  local sh = self.model.h/2
  local p1 = geo.Point:new(
      (-sw/2 * math.cos(dir_rad)) - (-sh/2 * math.sin(dir_rad)) + self.x,
      (-sw/2 * math.sin(dir_rad)) + (-sh/2 * math.cos(dir_rad)) + self.y)
  local p2 = geo.Point:new(
      ( sw/2 * math.cos(dir_rad)) - (-sh/2 * math.sin(dir_rad)) + self.x,
      ( sw/2 * math.sin(dir_rad)) + (-sh/2 * math.cos(dir_rad)) + self.y)
  local p3 = geo.Point:new(
      ( sw/2 * math.cos(dir_rad)) - ( sh/2 * math.sin(dir_rad)) + self.x,
      ( sw/2 * math.sin(dir_rad)) + ( sh/2 * math.cos(dir_rad)) + self.y)
  local p4 = geo.Point:new(
      (-sw/2 * math.cos(dir_rad)) - ( sh/2 * math.sin(dir_rad)) + self.x,
      (-sw/2 * math.sin(dir_rad)) + ( sh/2 * math.cos(dir_rad)) + self.y)
  return geo.Polygon:new {
    geo.Segment:new(p1, p2),
    geo.Segment:new(p2, p3),
    geo.Segment:new(p3, p4),
    geo.Segment:new(p4, p1)
  }
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
