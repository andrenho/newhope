require 'geo'

function test_PointEquality()
  local p1 = geo.Point:new(3, 4)
  local p2 = geo.Point:new(3, 4)
  assert(p1 == p2)
end

function test_PointDistance()
  local p1 = geo.Point:new(0, 0)
  local p2 = geo.Point:new(1, 1)
  local p3 = geo.Point:new(3, 3)
  assert(p1:distance(p2) == math.sqrt(2))
  assert(p1:distance_manhattan(p2) > p2:distance_manhattan(p3))
end

-- vim: ts=2:sw=2:sts=2:expandtab
