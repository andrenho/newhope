local Point = {}
Point.__index = Point

function Point:new(x, y)
   local self = setmetatable({}, Point)
   self.x = x
   self.y = y
   self.altitude = 0
   return self
end


function Point:unique_id()
   return self.y * 1000000 + self.x
end


-------------
-- PRIVATE --
-------------

function Point:__tostring()
   return ('[%0.2f %0.2f]'):format(self.x, self.y)
end

function Point:type() return 'Point' end


-------------------------------------------------------
-------------------------------------------------------


local Segment = {}
Segment.__index = Segment

function Segment:new(startpoint, endpoint)
   local self = setmetatable({}, Segment)
   self.startpoint, self.endpoint = startpoint, endpoint
   return self
end


function Segment:unique_id()
   return self.startpoint:unique_id() + self.endpoint:unique_id()
end


-------------
-- PRIVATE --
-------------

function Segment:__tostring()
   return '[Polygon]'
end


function Segment:type() return 'Segment' end

--------------------------------------------------------
--------------------------------------------------------


local Polygon = {}
Polygon.__index = Polygon

function Polygon:new(points, segments)
   local self = setmetatable({}, Polygon)
   self.points = points
   self.segments = segments
   self.altitude = 0
   self.outer_rectangle = {
      x1 = funct.min(funct.map(self.points, function(p) return p.x end)),
      y1 = funct.min(funct.map(self.points, function(p) return p.y end)),
      x2 = funct.max(funct.map(self.points, function(p) return p.x end)),
      y2 = funct.max(funct.map(self.points, function(p) return p.y end)),
   }
   return self
end


function Polygon:contains_point(x,y)
   local r = self.outer_rectangle
   local p = self.points
   local c = false
   if x >= r.x1 and y >= r.y1 and x < r.x2 and y < r.y2 then
      local j = #self.points
      for i=1,#self.points do
         if ((p[i].y > y) ~= (p[j].y > y)) and (x < (p[j].x-p[i].x) * (y - p[i].y) / (p[j].y - p[i].y) + p[i].x) then
            c = not c
         end
         j = i
      end
      return c
   end
   return false
end


-------------
-- PRIVATE --
-------------

function Polygon:__tostring()
   return '[Polygon]'
end

function Polygon:type() return 'Polygon' end

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
   self.points[pt:unique_id()] = pt
end


function Plane:add_segment(seg)
   self.segments[seg:unique_id()] = seg
end


function Plane:add_polygon(polygon)
   self.polygons[#self.polygons+1] = polygon
end


function Plane:point(x,y)
   return self.points[Point:new(x,y):unique_id()] -- TODO - slow
end


function Plane.generate_voronoi(polygons, repetitions, x1, y1, x2, y2)
   local plane = Plane:new()
   -- create voronoi
   local vor = voronoi:new(polygons, repetitions, x1, y1, x2, y2)
   for _,vpoly in ipairs(vor.polygons) do
      local pts = {}
      -- add points
      for j=1,#vpoly.points,2 do
         local x, y = vpoly.points[j], vpoly.points[j+1]
         local p = Point:new(x, y)
         pts[#pts+1] = p
         plane:add_point(p)
      end
      -- add segments
      local segs = {}
      for j=1,(#pts-1) do
         local seg = Segment:new(pts[j], pts[j+1])
         plane:add_segment(seg)
         segs[#segs+1] = seg
      end
      plane:add_segment(Segment:new(pts[#pts], pts[1]))
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

return {
   Point=Point,
   Segment=Segment,
   Polygon=Polygon,
   Plane=Plane,
}


-- vim: ts=3:sw=3:sts=3:expandtab
