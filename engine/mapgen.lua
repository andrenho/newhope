local MapGen = {}
MapGen.__index = MapGen

function MapGen:new(x1, y1, x2, y2, seed)
   local self = setmetatable({}, MapGen)
   self.polygons = {}
   self.__x1, self.__y1, self.__x2, self.__y2 = x1, y1, x2, y2
   self.__outer_rectangle = {}
   self.__points = {}
   math.randomseed(seed or os.time())
   return self
end


function MapGen:create()
   self.voronoi = self:__create_polygons()
   self:__set_altitude()
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
         self.__points[x][y] = 0
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


function MapGen:__set_altitude(seed)
   -- ideas from <http://www.stuffwithstuff.com/robot-frog/3d/hills/index.html>

   -- create heightmap
   local hm = {}
   for x=1,256 do 
      hm[x] = {}
      for y=1,256 do
         if x > 50 and y > 50 then
            hm[x][y] = 1
         else
            hm[x][y] = 0 
         end
      end
   end

   -- TODO - create altitude
   

   -- apply heightmap to points
   --local function closest_points
end


function MapGen:__create_biomes()
   for i,poly in ipairs(self.polygons) do
      poly.biome = Block.GRASS
   end
   self.polygons[5].biome = Block.WATER
   self.polygons[10].biome = Block.WATER
end


function MapGen:__polycontains(poly, x, y)
   -- TODO - this can be even faster if we create inner rectangles
   local r = poly.outer_rectangle
   if x >= r.x1 and x <= r.x2 and y >= r.y1 and y <= r.y2 then
      return poly.polygon:containspoint(x,y)
   end
   return false
end


function MapGen:__tostring()
   return '[MapGen]'
end


function MapGen:type() return 'MapGen' end


return MapGen

-- vim: ts=3:sw=3:sts=3:expandtab
