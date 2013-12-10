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

local function __create_shapes()
  local p = {
    geo.Point:new(0, 0), -- 1
    geo.Point:new(2, 0), -- 2
    geo.Point:new(2, 2), -- 3
    geo.Point:new(0, 2), -- 4
    geo.Point:new(1, 1), -- 5
    geo.Point:new(3, 0), -- 6
    geo.Point:new(4, 2), -- 7
    geo.Point:new(3, 3), -- 8
  }
  local shape1 = geo.Shape:new { p[1], p[2], p[3], p[4] }
  local shape2 = geo.Shape:new { p[5], p[6], p[7], p[8] }
  return p, shape1, shape2
end

function test_ShapeOuterRect()
  local p, shape1, shape2 = __create_shapes()
  local rect = shape2:outer_rectangle()
  assert(rect.x == 1)
  assert(rect.y == 0)
  assert(rect.w == 3)
  assert(rect.h == 3)
end

-- vim: ts=2:sw=2:sts=2:expandtab
