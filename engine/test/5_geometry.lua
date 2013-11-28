--
-- Chunk 5
--

function test_PolygonColision()
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
    geo.Line:new(p[1], p[2]),
    geo.Line:new(p[2], p[3]),
    geo.Line:new(p[3], p[4]),
    geo.Line:new(p[4], p[1]),
  }
  local line2 = {
    geo.Line:new(p[5], p[6]),
    geo.Line:new(p[6], p[7]),
    geo.Line:new(p[7], p[8]),
    geo.Line:new(p[8], p[5]),
  }
  local polygon1 = geo.Polygon:new { line1[1], line1[2], line1[3], line1[4] }
  local polygon2 = geo.Polygon:new { line2[1], line2[2], line2[3], line2[4] }
  assert(polygon1:intersect(polygon2))
end

-- vim: ts=2:sw=2:sts=2:expandtab
