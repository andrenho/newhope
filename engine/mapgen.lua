local MapGen = {}
MapGen.__index = MapGen

function MapGen:new(x1, y1, x2, y2, seed)
   local self = setmetatable({}, MapGen)
   self.polygons = {}
   self.rivers = {}
   self.__x1, self.__y1, self.__x2, self.__y2 = x1, y1, x2, y2
   self.__outer_rectangle = {}
   self.__points = {}
   self.point_list = {}
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
   self:__normalize_heightmap(hm, w, h)
   print('Applying heightmap...')
   self:__apply_heightmap(hm, w, h)
   print('Creating rivers...')
   self:__create_rivers()
   print('Identifying terrain features...')
   self:__terrain_features()
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
   for _=1,500 do
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
         if alt > -1 then
            hm[x2][y2] = set_to or (hm[x2][y2] + alt)
         end
      end
   end
end


function MapGen:__normalize_heightmap(hm, w, h)
   local max_alt = 0
   for x=0,255 do
      for y=0,255 do
         if hm[x][y] > max_alt then max_alt = hm[x][y] end
      end
   end
   for x=0,255 do
      for y=0,255 do
         hm[x][y] = hm[x][y] / max_alt * 255
      end
   end
end


function MapGen:__apply_heightmap(hm, w, h)
   local points = {}
   for x=0,255 do for y=0,255 do points[#points+1] = { x=x, y=y } end end

   local lim_x1, lim_y1, lim_x2, lim_y2 = world:limits()
   local prop_w, prop_h = lim_x1 / (lim_x2-lim_x1), lim_y1 / (lim_y2-lim_y1)

   for _,p in ipairs(self.plane.point_list) do
      local prop_x = (p.x / (lim_x2 - lim_x1) - prop_w) * w
      local prop_y = (p.y / (lim_y2 - lim_y1) - prop_h) * h
      local closest = { x = math.floor(prop_x), y = math.floor(prop_y) }
      self.plane.points[p].altitude = hm[closest.x][closest.y]
   end

   for _,poly in ipairs(self.plane.polygons) do
      poly.altitude = funct.avg(poly.points, function(p) return p.altitude end)
   end
end


function MapGen:__create_rivers()
   for _=1,15 do
      local p = self.plane:random_point()
      while p.altitude <= 0 do p = self.plane:random_point() end -- if it's on water, try a new point
      local points_used = { p } -- TODO - repeated?
      local river_pts = { p }
      --print(p)
      while p.altitude > 0 do
         -- find next point that contains the lowest altitude, ignoring the points already used
         local np, lowest_alt = nil, math.huge
         --print(unpack(self.plane:segments_containing_endpoint(p)))
         for _,seg in ipairs(self.plane:segments_containing_endpoint(p)) do
            -- find next point
            local op = seg.startpoint
            if op == p then op = seg.endpoint end
            -- find lowest point
            if not table.find(points_used, op) then
               if op.altitude < lowest_alt then
                  lowest_alt = op.altitude
                  np = op
               end
            end
         end
         if not np then break end -- ??

         p = np
         -- add segment
         points_used[#points_used+1] = p
         river_pts[#river_pts+1] = p
         if #river_pts > 100 then break end -- avoid infinite loops
      end
      self.rivers[#self.rivers+1] = river_pts
   end
end


function MapGen:__terrain_features()
   -- find moisture
   for _,pt in ipairs(self.plane.point_list) do
      local moisutre = 255
      -- TODO
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
