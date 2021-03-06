local World = {}
World.__index = World

--
-- create a new world map
-- 
function World:new(x1, y1, x2, y2)
   local self = setmetatable({}, World)

   self.x1, self.y1, self.x2, self.y2 = x1, y1, x2, y2

   self.objects = {}
   self.cities = {}
   self.predefined_tiles = {}

   self.__obj_ptr = {}
   self.__tile_cache = {}

   self.objects_ptr = {}
   return self
end


-- 
-- initialize world map
--
function World:initialize()
   -- create map
   self.mapgen = MapGen:new(self.x1, self.y1, self.x2, self.y2)
   self.mapgen:create()

   -- create cities
   self:__create_cities()
   self.mapgen:create_roads()

   self:__init_physics()

   self.player = self:__add_object(Player:new(5, 10))
   local players_vehicle = self:__add_object(Vehicle:new(10, 10, VehicleModel.REGULAR))
   self.player.vehicle = players_vehicle
   
   self:__add_objects_to_cities()

   self:__add_static_objects()
   collectgarbage()
end


--
-- clean up
--
function World:clean_up()
   for _,obj in ipairs(self.objects) do
      self:__remove_object(obj)
   end
   self:__finish_physics()
end


--
-- one step in the world
--
function World:step()
   self:__physics_step()
   for _,object in ipairs(self.objects) do
      object:step()
   end
end


--
-- return the stack of tiles (max 10)
--
function World:tiles(x, y)
   local predef = self.predefined_tiles[world:unique_tile_id(x,y)]
   if predef then
      return predef
   else
      for _, city in ipairs(self.cities) do
         if x >= city.x and x < (city.x+city.w) and y >= city.y and y < (city.y+city.h) then
            return city:tiles(x-city.x, y-city.y)
         end
      end
      return { self.mapgen:tile(x,y) } -- TODO
   end
end


--
-- return the stack of tile IDs (max 10)
--
function World:tilemap_ids(x1, y1, x2, y2)
   -- TODO - cache size
   local t = {}
   local __tile_cache = self.__tile_cache -- speedup hack
   for x = x1,x2 do
      if not __tile_cache[x] then __tile_cache[x] = {} end
      t[x] = {}
      for y = y1,y2 do
         local ids = __tile_cache[x][y]
         if not ids then
            ids = self:tiles(x,y)
            for k,v in ipairs(ids) do
               ids[k] = v.id
            end
            __tile_cache[x][y] = ids
         end
         t[x][y] = ids
      end
   end
   self.__tile_cache = __tile_cache
   return t
end


-- 
-- return a list of objects among the tiles
--
function World:objects_in_area(x1, y1, x2, y2)
   local cond = function(p) 
      local pos = p:pos()
      return (pos.x >= x1 and pos.x <= x2 and pos.y >= y1 and pos.y <= y2)
   end
   return funct.filter(self.objects, cond)
end


-- 
-- return a unique identifier for a tile, and revert the value
--
function World:unique_tile_id(x, y)
   return x + (y * 1000000)
end
function World:revert_unique_tile(id)
   return (id % 1000000), math.floor(id / World.W)
end


--
-- return if this tile can be walked by a person
--
function World:tile_walkable(x, y)
   local st = self:tiles(x, y)
   if st[1].friction == math.huge or (st[2] and st[2].solid) or (st[3] and st[3].solid) then
      return false
   end
   return true
end


--
-- when a collision happens, this method is called by the C code
--
function World:collision_callback(body_a, body_b)
   local a = self.__obj_ptr[body_a]
   local b = self.__obj_ptr[body_b]
   a:collision(b)
   b:collision(a)
end


-- 
-- Return the limits of the world (x1, y1, x2, y2)
-- 
function World:limits()
   return self.x1, self.y1, self.x2, self.y2
end


-- 
-- Return type
--
function World:type()
   return 'World'
end


--
-- Print some debug information
--
function World:debug()
   print 'something'
end


-------------
-- PRIVATE --
-------------

function World:__tostring()
   return '[World]'
end


function World:__create_cities()
   for _,point in ipairs(self.mapgen:cities_positions(20)) do
      if _ == 1 then point = geo.Point:new(0,0) end
      local biome = self.mapgen.plane:polygon_containing_point(point).biome
      local r, tp = math.random(), nil
      if r < 0.1 then
         tp = CityType.CAPITAL
      elseif r < 0.2 then
         tp = CityType.FRONTIER
      else
         tp = self:__city_type(biome)
      end
      if tp == CityType.RANDOM then tp = CityType.random() end
      self.cities[#self.cities+1] = City:new(math.ceil(point.x), math.ceil(point.y), tp, biome)
   end
end


function World:__city_type(b)
   if b == Block.GRASS then
      return CityType.AGRICULTURAL
   elseif b == Block.SNOW then
      return CityType.REFINERY
   elseif b == Block.BARE then
      return CityType.MINING
   elseif b == Block.TEMPFOR or b == Block.TROPFOR then
      return CityType.FORESTAL
   elseif b == Block.DESERT or b == Block.BEACH then
      return CityType.CHEMICAL
   end
   return CityType.RANDOM
end


function World:__add_objects_to_cities()
   for _,city in ipairs(self.cities) do
      for _,building in ipairs(city.buildings) do
         if building.layout.objects then
            for _,o in ipairs(building.layout.objects) do
               -- create object
               local obj = nil
               local x = o.x + city.x + building.x + 0.5
               local y = o.y + city.y + building.y + 0.5
               if o.type == 'Shopkeeper' then
                  obj = ai.Shopkeeper:new(x, y)
                  obj.city = city
               elseif o.type == 'CarDealer' then
                  obj = ai.CarDealer:new(x, y)
               elseif o.type == 'Bartender' then
                  obj = ai.Bartender:new(x, y)
               elseif o.type == 'Secretary' then
                  obj = ai.Secretary:new(x, y)
               elseif o.type == 'Dispatcher' then
                  obj = ai.Dispatcher:new(x, y)
               elseif o.type == 'Medic' then
                  obj = ai.Medic:new(x, y)
               end
               assert(obj, 'Invalid object type: ' .. o.type)
               self:__add_object(obj)
            end
         end
      end
   end
end


function World:__add_object(obj)
   self.objects[#self.objects+1] = obj
   obj:init_physics()
   self.__obj_ptr[obj.body] = obj
   return obj
end


function World:__remove_object(obj)
   obj:clean_up()
   for k,v in ipairs(obj) do 
      if v == obj then table.remove(self.objects, k) end
   end
end


-- TODO : add static objects outside cities
function World:__add_static_objects()
   for _,city in ipairs(self.cities) do
      for x = city.x, (city.x+city.w) do
         for y = city.y, (city.y+city.h)  do
            if not self:tile_walkable(x,y) then
               self:__physics_create_static_obj(x, y, 1, 1)
            end
         end
      end
   end
end

return World

-- vim: ts=3:sw=3:sts=3:expandtab
