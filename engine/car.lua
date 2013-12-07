local Car = {}
Car.__index = Car

function Car:new(x, y, model)
  local self = setmetatable({}, Car)
  self.x, self.y = x, y
  self.model = model
  self.direction = 0
  self.owner = nil
  self.movement = {
    accelerate = false,
    breaking = false,
    left = false,
    right = false
  }
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
      (-sw * math.sin(dir_rad)) + (-sh * math.cos(dir_rad)) + self.y,
      (-sw * math.cos(dir_rad)) - (-sh * math.sin(dir_rad)) + self.x)
  local p2 = geo.Point:new(
      ( sw * math.sin(dir_rad)) + (-sh * math.cos(dir_rad)) + self.y,
      ( sw * math.cos(dir_rad)) - (-sh * math.sin(dir_rad)) + self.x)
  local p3 = geo.Point:new(
      ( sw * math.sin(dir_rad)) + ( sh * math.cos(dir_rad)) + self.y,
      ( sw * math.cos(dir_rad)) - ( sh * math.sin(dir_rad)) + self.x)
  local p4 = geo.Point:new(
      (-sw * math.sin(dir_rad)) + ( sh * math.cos(dir_rad)) + self.y,
      (-sw * math.cos(dir_rad)) - ( sh * math.sin(dir_rad)) + self.x)
  return geo.Polygon:new {
    geo.Segment:new(p1, p2),
    geo.Segment:new(p2, p3),
    geo.Segment:new(p3, p4),
    geo.Segment:new(p4, p1)
  }
end

function Car:step()
  print(self.movement.accelerate)
  -- TODO
end

-------------
-- PRIVATE --
-------------

function Car:__tostring()
end

return Car

-- vim: ts=2:sw=2:sts=2:expandtab
