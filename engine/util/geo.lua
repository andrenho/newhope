local Point = {}
Point.__index = Point

function Point:new()
   local self = setmetatable({}, Point)
   return self
end

-------------
-- PRIVATE --
-------------

function Point:__tostring()
   return '[Point]'
end

function Point:type() return 'Point' end

return Point


-------------------------------------------------------
-------------------------------------------------------


local Segment = {}
Segment.__index = Segment

function Segment:new()
   local self = setmetatable({}, Segment)
   return self
end

-------------
-- PRIVATE --
-------------

function Segment:__tostring()
   return '[Polygon]'
end

function Segment:type() return 'Segment' end

return Segment


--------------------------------------------------------
--------------------------------------------------------


local Polygon = {}
Polygon.__index = Polygon

function Polygon:new(points, segments)
   self.points = points
   self.segments = segments
   return self
end


-------------
-- PRIVATE --
-------------

function Polygon:__tostring()
   return '[Polygon]'
end

function Polygon:type() return 'Polygon' end

return Polygon


-------------------------------------------------------
-------------------------------------------------------


local Plane = {}
Plane.__index = Plane


function Plane:new()
   local self = setmetatable({}, Plane)
   self.points = {}
   self.segments = {}
   self.polygons = {}
   return self
end


function Plane:add_point(pt)
end


function Plane:add_segment(seg)
end


function Plane:add_polygon(polygon)
   self.polygons[#self.polygons+1] = polygon
end


function Polygon.generate_voronoi(polygons, repetitions, x1, y1, x2, y2)
   local plane = Plane:new()
   -- create voronoi
   local vor = voronoi:new(polygons, repetitions, x1, y1, x2, y2)
   for _,vpoly in ipairs(vor.polygons) do
      local pts = {}
      -- add points
      for j=1,#vpoly.points,2 do
         local x, y = poly.points[j], poly.points[j+1]
         local p = point:new(x, y)
         pts[#pts+1] = p
         self:add_point(p)
      end
      -- add segments
      local segs = {}
      for j=1,(#pts-1) do
         local seg = Segment:new(pts[i], pts[i+1])
         self:add_segment(seg)
         segs[#segs+1] = seg
      end
      self:add_segment(Segment:new(pts[#pts], pts[1]))
      -- add polygons
      plane:add_polygon(Polygon:new(pts, segs))
   end
   return plane
end


-------------
-- PRIVATE --
-------------

function Plane:__tostring()
   return '[Plane]'
end

function Plane:type() return 'Plane' end

return Plane


-- vim: ts=3:sw=3:sts=3:expandtab
