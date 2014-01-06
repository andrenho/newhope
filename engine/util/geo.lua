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

function Polygon:new(points)
   local self = setmetatable({}, Polygon)
   self.points = points

   -- add segments
   self.segments = {}
   for i=1,(#points-1) do
      self.segments[#self.segments+1] = Segment:new(points[i], points[i+1])
   end
   self.segments[#self.segments+1] = Segment:new(points[#points], points[1])

   -- add outer rectangle
   self.polygons[i].outer_rectangle = {
      x1 = funct.min(points_x),
      y1 = funct.min(points_y),
      x2 = funct.max(points_x),
      y2 = funct.max(points_y),
   }

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


function Plane:find_segment(p1,p2)
end


function Plane:add_polygon(points)
   
end


function Polygon.generate_voronoi(polygons, repetitions, x1, y1, x2, y2)
   local plane = Plane:new()
   local vor = voronoi:new(polygons, repetitions, x1, y1, x2, y2)
   for _,vpoly in ipairs(vor.polygons) do
      local pts = {}
      for j=1,#vpoly.points,2 do
         local x, y = poly.points[j], poly.points[j+1]
         pts[#pts+1] = Point:new(x, y)
      end
      plane:add_polygon(pts)
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
