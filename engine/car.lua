local Car = {}
Car.__index = Car

function Car:new(x, y, model)
  local self = setmetatable({}, Car)
  self.x, self.y = x, y
  self.__model = model
  self.owner = nil
  self.controls = {
    accelerate = false,
    breaking = false,
    left = 0,
    right = 0,
  }
  self.physics = {
    speed = 0,
    front_wheel_angle = 0,
    rear_wheel_angle = 0,
    traction = true,
  }
  self.direction = 0
  self:__calculate_attributes()
  return self
end

--
-- Return the car polygon
-- 
function Car:polygon()
  local dir_rad = self.direction * math.pi / 180.0
  local sw = self.attrib.w/2
  local sh = self.attrib.h/2
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
  -- speed 0
  if self.physics.speed == 0 and not self.controls.accelerate then return end

  local fx, fy = self:__next_moving_point()
  -- TODO - check for collision
  self.x, self.y = fx, fy
  if self.owner.in_car then
    self.owner.x, self.owner.y = self.x, self.y
  end
end

-------------
-- PRIVATE --
-------------

function Car:__tostring()
end

function Car:__next_moving_point()
  return self.x+0.1, self.y -- TODO
end

function Car:__calculate_attributes()
  self.attrib = table.shallow_copy(self.__model)
end

return Car

-- vim: ts=2:sw=2:sts=2:expandtab
