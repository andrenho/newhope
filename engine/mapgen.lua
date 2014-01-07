local MapGen = {}
MapGen.__index = MapGen

function MapGen:new(x1, y1, x2, y2, seed)
   local self = setmetatable({}, MapGen)
   self.polygons = {}
   self.__x1, self.__y1, self.__x2, self.__y2 = x1, y1, x2, y2
   self.__outer_rectangle = {}
   self.__points = {}
   self.__point_list = {}
   self.__seed = seed
   return self
end


function MapGen:create()
   math.randomseed(self.__seed or os.time())
   print('Creating map polygons...')
   self.plane = self:__create_polygons()
   print('Creating heightmap...')
   local hm, w, h = self:__create_heightmap()
   self:__setup_heightmap_altitudes(hm, w, h)
   print('Applying heightmap...')
   self:__apply_heightmap(hm, w, h)
   print('Creating rivers...')
   self:__create_rivers()
   print('Creating biomes...')
   self:__create_biomes()
end


function MapGen:tile(x,y)
   for _,poly in ipairs(self.plane.polygons) do
      if poly:contains_point(x,y) then
         return poly.biome
      end
   end
   return Block.NOTHING
end


-------------
-- PRIVATE --
-------------

function MapGen:__create_polygons()
   return geo.Plane.generate_voronoi(500, 1, self.__x1, self.__y1, -self.__x1+self.__x2, -self.__y1+self.__y2)
end


function MapGen:__create_heightmap()
   local hm = {}
   for x=0,255 do 
      hm[x] = {}
      for y=0,255 do
         hm[x][y] = 0 
      end
   end
   return hm, 255, 255
end


function MapGen:__setup_heightmap_altitudes(hm, w, h)
   -- idea from <http://www.stuffwithstuff.com/robot-frog/3d/hills/index.html>
   for _=1,200 do
      local r = math.random(6, 30)
      local theta = math.random(0, 2*math.pi) + math.random()
      local distance = math.random(0, w/2-r*2)
      local x = math.floor(w / 2 + math.cos(theta) * distance)
      local y = math.floor(h / 2 + math.sin(theta) * distance)
      self:__create_hill(hm, w, h, x, y, r)
   end
end


function MapGen:__create_hill(hm, w, h, x1, y1, r, set_to)
   for x2=0,255 do
      for y2=0,255 do
         local alt = math.pow(r,2) - (math.pow(x2-x1,2) + math.pow(y2-y1,2))
         if alt > 0 then
            hm[x2][y2] = set_to or (hm[x2][y2] + alt)
         end
      end
   end
end


function MapGen:__apply_heightmap(hm, w, h)
   local points = {}
   for x=0,255 do for y=0,255 do points[#points+1] = { x=x, y=y } end end

   local lim_x1, lim_y1, lim_x2, lim_y2 = world:limits()
   local prop_w, prop_h = lim_x1 / (lim_x2-lim_x1), lim_y1 / (lim_y2-lim_y1)

   for _,poly in ipairs(self.plane.polygons) do
      for _,p in ipairs(poly.points) do
            local prop_x = (p.x / (lim_x2 - lim_x1) - prop_w) * w
            local prop_y = (p.y / (lim_y2 - lim_y1) - prop_h) * h
            local closest = self:__closest_point(points, prop_x, prop_y)
            self.plane:point(p.x,p.y).altitude = hm[closest.x][closest.y]
      end
      poly.altitude = funct.avg(poly.points, function(p) return p.altitude end)
   end
end


function MapGen:__create_rivers()
   local function create_river(x, y)
   end
   for _=1,1 do
      
   end
end


function MapGen:__create_biomes()
   for _,poly in ipairs(self.plane.polygons) do
      if poly.altitude > 0 then
         poly.biome = Block.GRASS
      else
         poly.biome = Block.WATER
      end
   end
   self.plane.polygons[1].biome = Block.GRASS
end


-- 
-- HELPER METHODS
--

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
