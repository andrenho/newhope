local MapGen = {}
MapGen.__index = MapGen

function MapGen:new(x1, y1, x2, y2)
   local self = setmetatable({}, MapGen)
   self.__x1, self.__y1, self.__x2, self.__y2 = x1, y1, x2, y2
   self.__outer_polygon = {}
   self.polygon_biome = {}
   return self
end


function MapGen:create()
   self.voronoi = self:__create_polygons()
   self:__create_biomes()
end


function MapGen:tile(x,y)
   for i,poly in ipairs(self.voronoi.polygons) do
      if self:__polycontains(poly, x, y) then
         return self.polygon_biome[i]
      end
   end
   return Block.NOTHING
end


-------------
-- PRIVATE --
-------------

function MapGen:__create_polygons()
   local vor = voronoi:new(200, 3, self.__x1, self.__y1, 
                             -self.__x1+self.__x2, -self.__y1+self.__y2)
   --for i,poly in ipairs(vor.polygons) do
   --   self.__outer_polygon[i] = 
   return vor
end


function MapGen:__create_biomes()
   for i = 1,#self.voronoi.polygons do
      self.polygon_biome[i] = Block.GRASS
   end
   self.polygon_biome[5] = Block.WATER
   self.polygon_biome[10] = Block.WATER
end


function MapGen:__polycontains(poly, x, y)
   -- TODO - this is too slow, try to make it faster
   return poly:containspoint(x,y)
end


function MapGen:__tostring()
   return '[MapGen]'
end


function MapGen:type() return 'MapGen' end


return MapGen

-- vim: ts=3:sw=3:sts=3:expandtab
