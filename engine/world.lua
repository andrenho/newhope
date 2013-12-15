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

  self.object_bodies = {}
  return self
end


-- 
-- initialize world map
--
function World:initialize()
  self.player = Player:new(0, 0)
  self:add_object(self.player)
  self.player:setup_collision_handler()

  self.cities[#self.cities+1] = City:new(1, 0, 0, 20, 20, Block.GRASS)
  self:__add_people_to_cities()

  self:add_object(Car:new(-5, -5, CarModel.REGULAR))

  self:__add_static_objects()
end


--
-- one step in the world
--
function World:step()
  for _,object in ipairs(self.objects) do
    object:step()
  end
end


--
-- Add a new object
--
function World:add_object(obj)
  self.objects[#self.objects+1] = obj
  obj:create_physics_body()
  self.object_bodies[obj.body] = obj
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
                 return (p.x >= x1 and p.x <= x2 and p.y >= y1 and p.y <= y2)
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
            person = StaticPerson:new(x, y)
          end
          assert(person, 'Invalid person type: ' .. p.type)
          self:add_object(person)
        end
      end
    end
  end
end

-- TODO : low performance, I think. Maybe add as the player walks?
function World:__add_static_objects()
  local x1, y1, x2, y2 = self:__limits()
  for x = x1,x2 do
    for y = y1,y2 do
      if not self:tile_walkable(x,y) then
        self:__physics_add_solid_tile(x, y, 1, 1)
      end
    end
  end
end

function World:__physics_add_solid_tile(x, y, w, h)
  error('This method must be implemented in C.')
end

function World:__limits()
  return -30, -30, 30, 30
end

return World

-- vim: ts=2:sw=2:sts=2:expandtab
