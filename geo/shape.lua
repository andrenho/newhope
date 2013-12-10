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
    for _,line_a in ipairs(self:lines()) do
      for _,line_b in ipairs(polygon:lines()) do
        if line_a:intersect(line_b) then return true end
      end
    end
  end
  return false
end

function Shape:outer_rectangle()
  error('TODO')
end

function Shape:translate(point)
  error('TODO')
end

function Shape:rotate(angle)
  error('TODO')
end

function Shape:lines()
  local lines = {}
  for i in 2,#self.points do
    lines[#lines+1] = { self.points[i-1], self.points[i] }
  end
  lines[#lines+1] = { self.points[#self.points], self.points[1] }
  return lines
end

function Shape:type()
  return Shape
end

-------------
-- PRIVATE --
-------------

function Shape:__tostring()
  return '[Shape]'
end

return Shape

-- vim: ts=2:sw=2:sts=2:expandtab
