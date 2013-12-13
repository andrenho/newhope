local Rectangle = {}
Rectangle.__index = Rectangle

function Rectangle:new(x, y, w, h)
  local self = setmetatable({}, Rectangle)
  self.x, self.y, self.w, self.h = x, y, w, h
  return self
end

function Rectangle:intersect(other, reverse)
  local points = {
    geo.Point:new(self.x, self.y), geo.Point:new(self.x+self.w, self.y),
    geo.Point:new(self.x+self.w, self.y+self.h), geo.Point:new(self.x+self.w, self.y+self.h)
  }
  for _,pt in ipairs(points) do
    if other:contains_point(pt) then return true end
  end
  if not reverse then 
    return other:intersect(self, true)
  else
    return false
  end
end

function Rectangle:contains_point(p)
  return (p.x >= self.x and p.x < self.x+self.w) and 
         (p.y >= self.y and p.y < self.y+self.h)
end

function Rectangle:type()
  return 'Rectangle'
end

-------------
-- PRIVATE --
-------------

function Rectangle:__tostring()
  return ('[Rect %0.2f %0.2f %0.2f %0.2f]'):format(self.x, self.y, self.w, self.h)
end

return Rectangle

-- vim: ts=2:sw=2:sts=2:expandtab
