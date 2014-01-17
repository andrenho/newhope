local MapGen = {}
MapGen.__index = MapGen

function MapGen:new(x1, y1, x2, y2, seed)
   local self = setmetatable({}, MapGen)
   self.polygons = {}
   self.rivers = {}
   self.__x1, self.__y1, self.__x2, self.__y2 = x1, y1, x2, y2
   self.__outer_rectangle = {}
   self.__points = {}
   self.__seed = seed
   self.__all_river_points = {}
   self.__river_tiles = objhash()
   self.point_list = {}
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
   print('Setting up rivers...')
   self:__create_river_tiles()
   print('Calculating moisture...')
   self:__calculate_moisture()
   print('Creating biomes...')
   self:__create_biomes()
   print('Adding beaches...')
   self:__add_beaches()
end


function MapGen:tile(x,y)
   local p = geo.Point:new(x,y)
   if self.__river_tiles[p] then return Block.WATER end
   for _,poly in ipairs(self.plane.polygons) do
      if poly:is_point_inside(x,y) then
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
      local x, y, r = self:__random_offcentre(w, h)
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
         hm[x][y] = hm[x][y] / max_alt
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
   for i=1,1 do
      ::try_again::

      -- choose a random point in land
      local p = self.plane:random_point()
      local points_used = { p }
      local river_pts = { p }

      while p.altitude > 0 do
         
         -- if the river is getting to sea, end it
         for _,poly in ipairs(self.plane.polygons) do
            for _,pt in ipairs(poly.points) do
               if table.find(river_pts, pt) then
                  if poly.altitude <= 0 then
                     goto done
                  end
               end
            end
         end

         -- find next point that contains the lowest altitude, ignoring the points already used
         local np, lowest_alt = nil, math.huge
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
         if not np then break end -- no point was found (TODO -- ??)

         -- add segment
         points_used[#points_used+1] = np
         river_pts[#river_pts+1] = np
         self.__all_river_points[#self.__all_river_points+1] = np

         if #river_pts > 100 then break end -- avoid infinite loops

         -- the point found is now the current point
         p = np
      end

      ::done::

      -- do not accept rivers of length 1 
      if #river_pts < 6 then 
         goto try_again 
      end

      -- add reiver
      self.rivers[#self.rivers+1] = river_pts
   end
end


function MapGen:__create_river_tiles()
   local plot = function(x,y)
      for xx=x-50,x+50, 3 do
         for yy=y-50,y+50, 3 do
            self.__river_tiles[geo.Point:new(xx,yy)] = true
         end
      end
   end
   for _,river in ipairs(self.rivers) do
      print('From '..tostring(river[1])..' to '..tostring(river[#river]))
      --for i = 1,(#river-1) do
      --   local j = i+1
         -- Bresenham's line algorithm
         -- TODO - fix for vertical line?
      local x0 = math.min(math.ceil(river[1].x), math.ceil(river[#river].x))
      local x1 = math.max(math.ceil(river[1].x), math.ceil(river[#river].x))
      local y0 = math.min(math.ceil(river[1].y), math.ceil(river[#river].y))
      local y1 = math.max(math.ceil(river[1].y), math.ceil(river[#river].y))
      -- the problem is when x0 > x1, but y1 > y0 - the slope must be positive
      print(x0, x1, y0, y1)
      local deltax = x1 - x0
      local deltay = y1 - y0
      local err = 0
      local deltaerr = math.abs(deltay / deltax)
      local y = y0
      for x=x0,x1 do
         plot(x,y)
         err = err + deltaerr
         if err >= 0.5 then
            y = y + 1
            err = err - 1
         end
      end
      --end
   end
end


function MapGen:__calculate_moisture()
   -- find moisture
   for _,pt in ipairs(self.plane.point_list) do
      if pt.altitude <= 0 then
         pt.moisture = 1
      else
         for _,ptw in ipairs(pt.closest_points) do
            if --[[ptw.altitude <= 0 or]] table.find(self.__all_river_points, ptw) then -- water
               pt.moisture = math.max((2000 - pt:distance(ptw)) / 2000, 0)
               break
            end
         end
      end
   end
   for _,poly in ipairs(self.plane.polygons) do
      poly.moisture = funct.avg(poly.points, function(p) return p.moisture end)
   end
end


function MapGen:__create_biomes()
--[[
ALT MOIST 1---------------------------0
 1        snow       tundra      bare
 |        temperF    shurbland  savannah
 0        tropF      grass      desert ]]
   for _,poly in ipairs(self.plane.polygons) do
      if poly.altitude <= 0 then
         poly.biome = Block.WATER
      elseif poly.altitude < 0.03 then
         if poly.moisture < 0.03 then
            poly.biome = Block.DESERT
         elseif poly.moisture < 0.2 then
            poly.biome = Block.GRASS
         else
            poly.biome = Block.TROPFOR
         end
      elseif poly.altitude < 0.15 then
         if poly.moisture < 0.05 then
            poly.biome = Block.SAVANNAH
         elseif poly.moisture < 0.1 then
            poly.biome = Block.SHRUBLAND
         else
            poly.biome = Block.TEMPFOR
         end
      else
         if poly.moisture < 0.05 then
            poly.biome = Block.BARE
         elseif poly.moisture < 0.09 then
            poly.biome = Block.TUNDRA
         else
            poly.biome = Block.SNOW
         end
      end
   end
end


function MapGen:__add_beaches()
   for _,poly in ipairs(self.plane.polygons) do
      if poly.biome ~= Block.WATER then
         for _,neighbour in ipairs(self.plane:polygon_neighbours(poly)) do
            if neighbour.biome == Block.WATER then
               poly.biome = Block.BEACH
               break
            end
         end
      end
   end
end


-- 
-- HELPER METHODS
--

function MapGen:__random_offcentre(w, h)
   local r = math.random(6, 30)
   local theta = math.random(0, 2*math.pi) + math.random()
   local distance = math.random(0, w/2-r*2)
   local x = math.floor(w / 2 + math.cos(theta) * distance)
   local y = math.floor(h / 2 + math.sin(theta) * distance)
   return x, y, r
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
