local World = {}
World.__index = World

--
-- create a new world map
-- 
function World:new()
   local self = setmetatable({}, World)

   self.objects = {}
   self.cities = {}
   self.predefined_tiles = {}

   self.__obj_ptr = {}

   self.objects_ptr = {}
   return self
end


-- 
-- initialize world map
--
function World:initialize()
   self:__init_physics()

   self.player = self:__add_object(Player:new(5, 10))
   local players_vehicle = self:__add_object(Vehicle:new(10, 10, VehicleModel.REGULAR))
   self.player.vehicle = players_vehicle
   
   self.cities[#self.cities+1] = City:new(CityLayout.LAYOUT_1, 0, 0, 20, 20, Block.GRASS)
   self:__add_people_to_cities()

   self:__add_static_objects()
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
      return { Block.GRASS } -- TODO
   end
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

function World:__add_people_to_cities()
   for _,city in ipairs(self.cities) do
      for _,building in ipairs(city.buildings) do
         if building.layout.people then
            for _,p in ipairs(building.layout.people) do
               -- create person
               local person = nil
               local x = p.x + city.x + building.x + 0.5
               local y = p.y + city.y + building.y + 0.5
               if p.type == 'Shopkeeper' then
                  person = ai.Shopkeeper:new(x, y) -- TODO (?)
                  self:__add_object(person)
               end
               assert(person, 'Invalid person type: ' .. p.type)
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


-- TODO : low performance, I think. Maybe add as the player walks?
function World:__add_static_objects()
   local x1, y1, x2, y2 = self:__limits()
   for x = x1,x2 do
      for y = y1,y2 do
         if not self:tile_walkable(x,y) then
            self:__physics_create_static_obj(x, y, 1, 1)
         end
      end
   end
end

function World:__limits()
   return -30, -30, 30, 30
end

return World

-- vim: ts=3:sw=3:sts=3:expandtab
