local MapGen = {}
MapGen.__index = MapGen

function MapGen:new(x1, y1, x2, y2, seed)
   local self = setmetatable({}, MapGen)
   self.polygons = {}
   self.__x1, self.__y1, self.__x2, self.__y2 = x1, y1, x2, y2
   self.__outer_rectangle = {}
   self.__points = {}
   self.__seed = seed
   return self
end


function MapGen:create()
   math.randomseed(self.__seed or os.time())
   print('Creating map polygons...')
   self.voronoi = self:__create_polygons()
   print('Creating heightmap...')
   local hm, w, h = self:__create_heightmap()
   self:__setup_heightmap_altitudes(hm, w, h)
   print('Applying heightmap...')
   self:__apply_heightmap(hm, w, h)
   print('Creating biomes...')
   self:__create_biomes()
end


function MapGen:tile(x,y)
   for i,poly in ipairs(self.polygons) do
      if self:__polycontains(poly, x, y) then
         return poly.biome
      end
   end
   return Block.NOTHING
end


-------------
-- PRIVATE --
-------------

function MapGen:__create_polygons()
   -- create voronoi diagram
   local vor = voronoi:new(500, 1, self.__x1, self.__y1, 
                             -self.__x1+self.__x2, -self.__y1+self.__y2)
   for i,poly in ipairs(vor.polygons) do
      -- create polygon
      self.polygons[i] = {}
      self.polygons[i].polygon = poly

      -- find outer rectangles
      local points_x, points_y = {}, {}
      for j=1,#poly.points,2 do
         local x, y = poly.points[j], poly.points[j+1]
         points_x[#points_x+1] = x
         points_y[#points_y+1] = y
         if not self.__points[x] then self.__points[x] = {} end
      end
      self.polygons[i].outer_rectangle = {
         x1 = funct.min(points_x),
         y1 = funct.min(points_y),
         x2 = funct.max(points_x),
         y2 = funct.max(points_y),
      }
   end

   return vor
end


function MapGen:__create_heightmap()
   local hm = {}
   for x=0,255 do 
      hm[x] = {}
      for y=0,255 do
         if x > 50 and y > 50 then
            hm[x][y] = 1
         else
            hm[x][y] = 0 
         end
      end
   end
   return hm, 255, 255
end


function MapGen:__setup_heightmap_altitudes(hm, w, h, seed)
   -- ideas from <http://www.stuffwithstuff.com/robot-frog/3d/hills/index.html>
end


function MapGen:__apply_heightmap(hm, w, h)
   local points = {}
   for x=0,255 do for y=0,255 do points[#points+1] = { x=x, y=y } end end

   local lim_x1, lim_y1, lim_x2, lim_y2 = world:limits()
   local prop_w, prop_h = lim_x1 / (lim_x2-lim_x1), lim_y1 / (lim_y2-lim_y1)
   for _,poly in ipairs(self.polygons) do
      for j=1,#poly.polygon.points,2 do
         local x, y = poly.polygon.points[j], poly.polygon.points[j+1]
         if not self.__points[x][y] then
            local prop_x = (x / (lim_x2 - lim_x1) - prop_w) * w
            local prop_y = (y / (lim_y2 - lim_y1) - prop_h) * h
            local closest = self:__closest_point(points, prop_x, prop_y)
            self.__points[x][y] = hm[closest.x][closest.y]
         end
         assert(self.__points[x][y])
      end
   end

   -- calculate polygon average altitude
   for _,poly in ipairs(self.polygons) do
      local i, alt = 0, 0
      for j=1,#poly.polygon.points,2 do
         local x, y = poly.polygon.points[j], poly.polygon.points[j+1]
         alt = alt + self.__points[x][y]
         i = i+1
      end
      assert(i>0)
      poly.altitude = alt / i
   end
end


function MapGen:__create_biomes()
   for i,poly in ipairs(self.polygons) do
      if poly.altitude > 0 then
         poly.biome = Block.GRASS
      else
         poly.biome = Block.WATER
      end
   end
end


-- 
-- HELPER METHODS
--

function MapGen:__polycontains(poly, x, y)
   -- TODO - this can be even faster if we create inner rectangles
   local r = poly.outer_rectangle
   if x >= r.x1 and x <= r.x2 and y >= r.y1 and y <= r.y2 then
      return poly.polygon:containspoint(x,y)
   end
   return false
end


function MapGen:__closest_point(pts, x, y)
   local min_dist, min_pt = math.huge, { x = -math.huge, y = -math.huge }
   for _,pt in ipairs(pts) do
      local dst = math.abs(pt.x - x) + math.abs(pt.y - y)
      if dst < min_dist then
         min_dist = dst
         min_pt.x, min_pt.y = pt.x, pt.y
      end
   end
   assert(min_dist ~= math.huge)
   return min_pt
end


function MapGen:__tostring()
   return '[MapGen]'
end


function MapGen:type() return 'MapGen' end


return MapGen

-- vim: ts=3:sw=3:sts=3:expandtab