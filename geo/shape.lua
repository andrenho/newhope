local Shape = {}
Shape.__index = Shape

function Shape:new(anchor, points)
  local self = setmetatable({}, Shape)
  self.points = points
  return self
end

function Shape:contains_point(p)
  error('TODO')
end

function Shape:intersect(other)
  if self:outer_rectangle():intersect(other:outer_rectangle()) then
    for _,line_a in ipairs(self:segments()) do
      for _,line_b in ipairs(polygon:segments()) do
        if line_a:intersect(line_b) then return true end
      end
    end
  end
  return false
end

function Shape:outer_rectangle()
  local rect = geo.Rectangle:new(
    funct.min(funct.map(self:points(), function(p) return p.x end)),
    funct.min(funct.map(self:points(), function(p) return p.y end)),
    0, 0
  )
  rect.w = funct.max(funct.map(self:points(), function(p) return p.x end)) - rect.x
  rect.h = funct.max(funct.map(self:points(), function(p) return p.y end)) - rect.y
  return rect
end

function Shape:translate(point)
  error('TODO')
end

function Shape:rotate(angle)
  error('TODO')
end

function Shape:segments()
  local lines = {}
  for i in 2,#self.points do
    lines[#lines+1] = geo.Segment:new(self.points[i-1], self.points[i])
  end
  lines[#lines+1] = geo.Segment:new(self.points[#self.points], self.points[1])
  return lines
end

function Shape:type()
  return 'Shape'
end

-------------
-- PRIVATE --
-------------

function Shape:__tostring()
  return '[Shape]'
end

return Shape

-- vim: ts=2:sw=2:sts=2:expandtab
