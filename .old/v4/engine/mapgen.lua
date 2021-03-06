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
   self.__river_tiles = {}
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


-- return positions for new cities
function MapGen:cities_positions(n)
   local t, ignore = {}, {}
   ::again::
   for _,biome in ipairs(Block.terrains) do -- cycle between terrains
      if n <= 0 then break end -- done
      if biome ~= Block.WATER and biome ~= Block.BEACH then
         local poly, tries = nil, 0
         while not poly or poly.biome ~= biome do
            poly = self.plane:random_polygon()
            if biome == Block.DESERT and poly.biome == Block.BEACH then break end -- DESERT == BEACH
            if table.find(ignore, poly) then poly = nil end -- ignore polygons already included
            tries = tries + 1
            if tries > 100 then goto next_biome end -- terrain not found, try next
         end
         if poly then 
            t[#t+1] = poly.center_point -- add polygon to list
            ignore[#ignore+1] = poly
            for _,p in ipairs(self.plane:polygon_neighbours(poly)) do ignore[#ignore+1] = p end -- ignore neighbours
         end
         n = n - 1
      end
      ::next_biome::
   end
   if n > 0 then goto again end
   return t
end


function MapGen:create_roads()
end


function MapGen:tile(x,y)
   local p = geo.Point:new(x,y)
   if self.__river_tiles[x] and self.__river_tiles[x][y] then return Block.WATER end
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
   for i=1,12 do
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

      -- do not accept rivers of length 6
      if #river_pts < 6 then 
         goto try_again 
      end

      -- add river
      self.rivers[#self.rivers+1] = river_pts
   end
end


function MapGen:__create_river_tiles()
   local add_tile = function(x,y)
      if x ~= x or y ~= y then return end -- test for NaN
      if not self.__river_tiles[x] then self.__river_tiles[x] = {} end
      self.__river_tiles[x][y] = true
   end
   local plot = function(x0,y0)
      local x, y = math.random(3,12), 0
      local radius_error = 1-x
      while x >= y do
         for nx = -x,x do add_tile(nx+x0, y+y0) end
         for ny = -y,y do add_tile(x+x0, ny+y0) end
         for nx = -x,x do add_tile(nx+x0, -y+y0) end
         for ny = -y,y do add_tile(-x+x0, ny+y0) end
         y = y+1
         if radius_error < 0 then
            radius_error = radius_error + (2*y+1)
         else
            x = x-1
            radius_error = radius_error + (2*(y-x+1))
         end
      end
   end
   for _,river in ipairs(self.rivers) do
      for i = 1,(#river-1) do
         local x1, x2, y1, y2 = math.ceil(river[i].x), math.ceil(river[i+1].x),
                                math.ceil(river[i].y), math.ceil(river[i+1].y)
         local dx, dy = x2-x1, y2-y1
         local v = (x1>x2) and -1 or 1
         for x=x1,x2,v do
            local y = y1 + dy * (x - x1) / dx
            plot(x, math.ceil(y))
         end
      end
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
         if poly.moisture < 0.05 then
            poly.biome = Block.DESERT
         elseif poly.moisture < 0.2 then
            poly.biome = Block.GRASS
         else
            poly.biome = Block.TROPFOR
         end
      elseif poly.altitude < 0.2 then
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
