local MapGen = {}
MapGen.__index = MapGen

function MapGen:new(x1, y1, x2, y2)
   local self = setmetatable({}, MapGen)
   self.__x1, self.__y1, self.__x2, self.__y2 = x1, y1, x2, y2
   self.__outer_rectangle = {}
   self.polygon_biome = {}
   return self
end


function MapGen:create()
   self.voronoi = self:__create_polygons()
   self:__create_biomes()
end


function MapGen:tile(x,y)
   for i,poly in ipairs(self.voronoi.polygons) do
      if self:__polycontains(i, x, y) then
         return self.polygon_biome[i]
      end
   end
   return Block.NOTHING
end


-------------
-- PRIVATE --
-------------

function MapGen:__create_polygons()
   -- create voronoi diagram
   local vor = voronoi:new(500, 2, self.__x1, self.__y1, 
                             -self.__x1+self.__x2, -self.__y1+self.__y2)
   -- find outer rectangles
   for i,poly in ipairs(vor.polygons) do
      local points_x, points_y = {}, {}
      for j=1,#poly.points,2 do
         points_x[#points_x+1] = poly.points[j]
         points_y[#points_y+1] = poly.points[j+1]
      end
      self.__outer_rectangle[i] = {
         x1 = funct.min(points_x),
         y1 = funct.min(points_y),
         x2 = funct.max(points_x),
         y2 = funct.max(points_y),
      }
   end

   return vor
end


function MapGen:__create_biomes()
   for i = 1,#self.voronoi.polygons do
      self.polygon_biome[i] = Block.GRASS
   end
   self.polygon_biome[5] = Block.WATER
   self.polygon_biome[10] = Block.WATER
end


function MapGen:__polycontains(n_poly, x, y)
   -- TODO - this can be even faster if we create inner rectangles
   local r = self.__outer_rectangle[n_poly]
   if x >= r.x1 and x <= r.x2 and y >= r.y1 and y <= r.y2 then
      return self.voronoi.polygons[n_poly]:containspoint(x,y)
   end
   return false
end


function MapGen:__tostring()
   return '[MapGen]'
end


function MapGen:type() return 'MapGen' end


return MapGen

-- vim: ts=3:sw=3:sts=3:expandtab
