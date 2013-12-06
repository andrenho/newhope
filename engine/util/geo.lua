
----------------------- POINT ------------------------

local Point = {}
Point.__index = Point

function Point:new(x, y)
  local self = setmetatable({}, Point)
  self.x, self.y = x, y
  return self
end

function Point:__eq(other)
  return self.x == other.x and self.y == other.y
end

function Point:__tostring()
  return ('{ x=%0.2f, y=%0.2f }'):format(self.x, self.y)
end

----------------------- LINE -------------------------

local Segment = {}
Segment.__index = Segment

function Segment:new(p1, p2)
  local self = setmetatable({}, Segment)
  self.p1, self.p2 = p1, p2
  return self
end

function Segment:intersect(other)
  -- check if lines are the same
  if self == other then return true end
  -- check if lines are parallel
  if self:slope() == other:slope() then return false end
  -- check if segments touch
  local is_on_segment = function(xi, yi, xj, yj, xk, yk)
    return (xi <= xk or xj <= xk) and (xk <= xi or xk <= xj) and
           (yi <= yk or yj <= yk) and (yk <= yi or yk <= yj)
  end
  local compute_direction = function(xi, yi, xj, yj, xk, yk)
    local a = (xk - xi) * (yj - yi);
    local b = (xj - xi) * (yk - yi);
    if a < b then
      return -1
    elseif a > b then
      return 1
    else
      return 0
    end
  end
  local x1, x2, x3, x4 = self.p1.x, self.p2.x, other.p1.x, other.p2.x
  local y1, y2, y3, y4 = self.p1.y, self.p2.y, other.p1.y, other.p2.y
  local d1 = compute_direction(x3, y3, x4, y4, x1, y1)
  local d2 = compute_direction(x3, y3, x4, y4, x2, y2)
  local d3 = compute_direction(x1, y1, x2, y2, x3, y3)
  local d4 = compute_direction(x1, y1, x2, y2, x4, y4)
  return (((d1 > 0 and d2 < 0) or (d1 < 0 and d2 > 0)) and
          ((d3 > 0 and d4 < 0) or (d3 < 0 and d4 > 0))) or
         (d1 == 0 and is_on_segment(x3, y3, x4, y4, x1, y1)) or
         (d2 == 0 and is_on_segment(x3, y3, x4, y4, x2, y2)) or
         (d3 == 0 and is_on_segment(x1, y1, x2, y2, x3, y3)) or
         (d4 == 0 and is_on_segment(x1, y1, x2, y2, x4, y4))
end

function Segment:slope()
  return (self.p2.y - self.p1.y) / (self.p2.x - self.p1.x)
end

function Segment:__eq(other)
  return (self.p1 == other.p1 and self.p2 == other.p2)
      or (self.p1 == other.p2 and self.p2 == other.p1)
end

function Segment:__tostring()
  return ('[Segment %s %s]'):format(self.p1, self.p2)
end

---------------------- RECTANGLE ----------------------
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

function Rectangle:__tostring()
  return ('[Rect %0.2f %0.2f %0.2f %0.2f]'):format(self.x, self.y, self.w, self.h)
end

---------------------- POLYGON ------------------------

local Polygon = {}
Polygon.__index = Polygon

function Polygon:new(lines)
  local self = setmetatable({}, Polygon)
  self.lines = lines
  return self
end

function Polygon:intersect(polygon)
  if self:outer_rectangle():intersect(polygon:outer_rectangle()) then
    for _,line_a in ipairs(self.lines) do
      for _,line_b in ipairs(polygon.lines) do
        if line_a:intersect(line_b) then return true end
      end
    end
  end
  return false
end

function Polygon:outer_rectangle()
  local rect = Rectangle:new(
    funct.min(funct.map(self:points(), function(p) return p.x end)),
    funct.min(funct.map(self:points(), function(p) return p.y end)),
    0, 0
  )
  rect.w = funct.max(funct.map(self:points(), function(p) return p.x end)) - rect.x
  rect.h = funct.max(funct.map(self:points(), function(p) return p.y end)) - rect.y
  return rect
end

function Polygon:points()
  local a = {}
  for _,line in ipairs(self.lines) do
    a[line.p1] = true
    a[line.p2] = true
  end
  local b = {}
  for p,_ in pairs(a) do b[#b+1] = p end
  return b
end

function Polygon:__tostring()
  local s = { '[Polygon\n' }
  for _,line in ipairs(self.lines) do
    s[#s+1] = '  '..tostring(line)..'\n'
  end
  s[#s+1] = ']'
  return table.concat(s)
end

---------------------- RETURN --------------------------

return {
  Point = Point,
  Segment = Segment,
  Polygon = Polygon,
}

-- vim: ts=2:sw=2:sts=2:expandtab
