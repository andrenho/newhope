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

function test_SegmentIntersect()
  local p, line1, line2, shape1, shape2 = __create_shapes()
  local l1, l2, l3 = geo.Segment:new(p[1], p[8]), geo.Segment:new(p[8], p[1]),
                     geo.Segment:new(p[2], p[6])
  assert(l1:intersect(l2))
  assert(not l1:intersect(l3))
end

function test_SegmentIntersect2()
  local p1, p2, p3, p4 = geo.Point:new(8,11), geo.Point:new(12,11),
                         geo.Point:new(9,10.5), geo.Point:new(9,11.5)
  assert(geo.Segment:new(p1,p2):intersect(geo.Segment:new(p3,p4)))
end

function test_ShapeIntersect()
  local p, shape1, shape2 = __create_shapes()
  assert(shape1:intersect(shape1))
  assert(shape1:intersect(shape2))
end

function test_ShapeIntersect2()
  local pa1, pa2, pa3, pa4 = geo.Point:new(12,9), geo.Point:new(12,11),
                             geo.Point:new(8,11), geo.Point:new(8,9)
  local pb1, pb2, pb3, pb4 = geo.Point:new(9,10.5), geo.Point:new(10,10.5),
                             geo.Point:new(10,11.5), geo.Point:new(9,11.5)
  local p1 = geo.Shape:new { pa1, pa2, pa3, pa4 }
  local p2 = geo.Shape:new { pb1, pb2, pb3, pb4 }
  assert(p1:intersect(p2))
  assert(p2:intersect(p1))
end

function test_Vector()
  local v1 = geo.Vector:new_mag(1, math.pi/4)
  local v2 = geo.Vector:new_xy(v1.x, v1.y)
  assert(v1 == v2)
  local v3 = geo.Vector:new_xy(5, 5)
  local v4 = geo.Vector:new_mag(v3.magnitude, v3.angle)
  assert(v3 == v4)
  local p = geo.Point:new(1,1)
  local v = geo.Vector:new_xy(1,1)
  local p2 = p:translate(v)
  assert(p2.x == 2)
  assert(p2.y == 2)
end

-- vim: ts=2:sw=2:sts=2:expandtab
