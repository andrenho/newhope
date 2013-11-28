
----------------------- POINT ------------------------

local Point = {}
Point.__index = Point

function Point:new(x, y)
  local self = setmetatable({}, Point)
  self.x, self.y = x, y
  return self
end

function Point:__tostring()
  return ('{ x=%0.2f, y=%0.2f }'):format(self.x, self.y)
end

----------------------- LINE -------------------------

local Line = {}
Line.__index = Line

function Line:new(p1, p2)
  local self = setmetatable({}, Line)
  self.p1, self.p2 = p1, p2
  return self
end

function Line:__tostring()
  return ('[Line %s %s]'):format(self.p1, self.p2)
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
  if self:__rect_intersect(polygon) then
    return true -- TODO
    --[[for line_a in self.lines do
      for line_b in polygon.lines do
        if line_a:intersect(line_b) then return true end
      end
    end]]
  end
  return false
end

function Polygon:__rect_intersect(polygon)

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
  Line = Line,
  Polygon = Polygon,
}

-- vim: ts=2:sw=2:sts=2:expandtab
