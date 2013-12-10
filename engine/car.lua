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
    gear = 1,
  }
  self:__calculate_attributes()
  self.__physics = CarPhysics:new(self.controls, self.attrib)
  return self
end

--
-- Return the car polygon
-- 
function Car:polygon()
  local dir_rad = self.__physics.direction * math.pi / 180.0
  local sw = self.attrib.w/2
  local sh = self.attrib.h/2
  local p1 = geo.Point:new(
      (-sw * math.sin(dir_rad)) + (-sh * math.cos(dir_rad)) + self.x,
      (-sw * math.cos(dir_rad)) - (-sh * math.sin(dir_rad)) + self.y)
  local p2 = geo.Point:new(
      ( sw * math.sin(dir_rad)) + (-sh * math.cos(dir_rad)) + self.x,
      ( sw * math.cos(dir_rad)) - (-sh * math.sin(dir_rad)) + self.y)
  local p3 = geo.Point:new(
      ( sw * math.sin(dir_rad)) + ( sh * math.cos(dir_rad)) + self.x,
      ( sw * math.cos(dir_rad)) - ( sh * math.sin(dir_rad)) + self.y)
  local p4 = geo.Point:new(
      (-sw * math.sin(dir_rad)) + ( sh * math.cos(dir_rad)) + self.x,
      (-sw * math.cos(dir_rad)) - ( sh * math.sin(dir_rad)) + self.y)
  return geo.Polygon:new {
    geo.Segment:new(p1, p2),
    geo.Segment:new(p2, p3),
    geo.Segment:new(p3, p4),
    geo.Segment:new(p4, p1)
  }
end

function Car:step()
  -- ask to the physics module calcluate step size and direction, then move
  local movement_vector = self.__physics:next_frame()
  local fp = movement_vector:terminal_point(geo.Point:new(self.x, self.y))
  self.x, self.y = fp.x, fp.y
  if self.owner.in_car then
    self.owner.x, self.owner.y = self.x, self.y
  end
end

function Car:direction() 
  return self.__physics.direction 
end

-------------
-- PRIVATE --
-------------

function Car:__tostring()
  return '[Car x:' .. self.x .. ' y:' .. self.y .. ']'
end

function Car:__calculate_attributes()
  self.attrib = table.shallow_copy(self.__model)
end

return Car

-- vim: ts=2:sw=2:sts=2:expandtab
