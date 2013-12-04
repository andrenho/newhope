--
-- Chunk 5 - Geometry
--

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
  local line1 = {
    geo.Segment:new(p[1], p[2]),
    geo.Segment:new(p[2], p[3]),
    geo.Segment:new(p[3], p[4]),
    geo.Segment:new(p[4], p[1]),
  }
  local line2 = {
    geo.Segment:new(p[5], p[6]),
    geo.Segment:new(p[6], p[7]),
    geo.Segment:new(p[7], p[8]),
    geo.Segment:new(p[8], p[5]),
  }
  local polygon1 = geo.Polygon:new { line1[1], line1[2], line1[3], line1[4] }
  local polygon2 = geo.Polygon:new { line2[1], line2[2], line2[3], line2[4] }
  return p, line1, line2, polygon1, polygon2
end

function test_ElementsEquality()
  local p1, p2, p3 = geo.Point:new(1, 2), geo.Point:new(1, 2), geo.Point:new(2, 1)
  local l1, l2 = geo.Segment:new(p2, p3), geo.Segment:new(p3, p2)
  assert(p1 == p2)
  assert(l1 == l2)
end

function test_PolygonPoints()
  local p, line1, line2, polygon1, polygon2 = __create_shapes()
  local ppts = polygon1:points()
  for i = 1,4 do
    assert(table.find(ppts, p[i]))
  end
end

function test_PolygonOuterRect()
  local p, line1, line2, polygon1, polygon2 = __create_shapes()
  local rect = polygon2:outer_rectangle()
  assert(rect.x == 1)
  assert(rect.y == 0)
  assert(rect.w == 3)
  assert(rect.h == 3)
end

function test_SegmentIntersect()
  local p, line1, line2, polygon1, polygon2 = __create_shapes()
  local l1, l2, l3 = geo.Segment:new(p[1], p[8]), geo.Segment:new(p[8], p[1]),
                     geo.Segment:new(p[2], p[6])
  assert(l1:intersect(l2))
  assert(not l1:intersect(l3))
end

function test_PolygonIntersect()
  local p, line1, line2, polygon1, polygon2 = __create_shapes()
  assert(polygon1:intersect(polygon1))
  assert(polygon1:intersect(polygon2))
end

-- vim: ts=2:sw=2:sts=2:expandtab
