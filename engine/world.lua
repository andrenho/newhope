local World = {}
World.__index = World

--
-- create a new world map
-- 
function World:new(w, h)
  if #callback.missing > 0 then 
    error('Not all callbacks were installed! Missing: '..table.concat(callback.missing, ', '))
  end
  local self = setmetatable({}, World)
  self.w = w
  self.h = h
  self.talk_queue = TalkQueue:new()
  self.player = Player:new(7, 7)
  self.people = { self.player }
  self.cars = { Car:new(10, 10, CarModel.REGULAR) }
  self.player:acquire_car(self.cars[1])
  self.cities = { City:new(1, 0, 0, 20, 20, Block.GRASS) }
  self.predefined_tiles = {}
  self:__add_people_to_cities()
  return self
end


--
-- one step in the world
--
function World:step()
  for _,person in ipairs(self.people) do
    person:step()
  end
  for _,car in ipairs(self.cars) do
    car:step()
  end
end


--
-- return the stack of tiles (max 10)
--
function World:tiles(x, y)
  local predef = self.predefined_tiles[world:unique_tile_id(x,y)]
  if x < 0 or y < 0 or x > (self.w-1) or y > (self.h-1) then
    return { Block.WATER }
  elseif predef then
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
-- Return the person in a given position. If no person is there, return nil.
--
function World:person_in_position(x, y, except)
  local people = self:people_in_area(x-1, y-1, x+1, y+1)
  if people[1] == except then
    return people[2]
  else
    return people[1]
  end
end

-- 
-- return a list of people among the tiles
--
function World:people_in_area(x1, y1, x2, y2)
  local cond = function(p) 
                 return (p.x >= x1 and p.x <= x2 and p.y >= y1 and p.y <= y2 
                         and not p.in_car)
               end
  return funct.filter(self.people, cond)
end


-- 
-- return a list of cars among the tiles
--
function World:cars_in_area(x1, y1, x2, y2)
  local cond = function(p) return (p.x >= x1 and p.x <= x2 and p.y >= y1 and p.y <= y2) end
  return funct.filter(self.cars, cond)
end


-- 
-- return a unique identifier for a tile, and revert the value
--
function World:unique_tile_id(x, y)
  return x + (y * self.w)
end
function World:revert_unique_tile(id)
  return (id % self.w), math.floor(id / self.w)
end


--
-- return if this tile can be walked by a person
--
World.crossable = { Block.NOTHING, Block.DOOR_OPEN }
function World:tile_walkable(x, y)
  local st = self:tiles(x, y)
  if st[1] == Block.WATER or st[1] == Block.NOTHING then
    return false
  end
  for _, cblock in ipairs(World.crossable) do
    if #st >= 2 and st[2] == cblock then st[2] = Block.NOTHING end
    if #st >= 3 and st[3] == cblock then st[3] = Block.NOTHING end
  end
  if #st > 1 and (st[2] ~= Block.NOTHING or st[3] ~= Block.NOTHING) then
    return false
  end
  return true
end


-------------
-- PRIVATE --
-------------

function World:__tostring()
  return '[World w:' .. self.w .. ' h:' .. self.h .. ']'
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
            person = Shopkeeper:new(x, y)
          end
          assert(person, 'Invalid person type: ' .. p.type)
          self.people[#self.people+1] = person
        end
      end
    end
  end
end

return World

-- vim: ts=2:sw=2:sts=2:expandtab
