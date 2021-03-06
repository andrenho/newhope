local Segment = {}
Segment.__index = Segment

function Segment:new(p1, p2)
  local self = setmetatable({}, Segment)
  self.p1, self.p2 = p1, p2
  return self
end

function Segment:type()
  return 'Segment'
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

return Segment

-- vim: ts=2:sw=2:sts=2:expandtab
