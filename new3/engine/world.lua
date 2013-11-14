--
-- packages
--

-- load modules (globals!!!)
require('util.strict')
mod    = require('util.modules')
funct  = mod.require('util.funct')
table  = mod.require('util.table')
string = mod.require('util.string')
msg    = mod.require('msg')

-- load classes
local Block    = mod.require('block')
local Person   = mod.require('person')
local City     = mod.require('city')
local Building = mod.require('building')

--
-- class World
-- 
local World = {}
World.__index = World

--
-- create a new world map
-- 
function World:new(w, h)
  local self = setmetatable({}, World)
  self.w = w
  self.h = h
  self.hero = Person:new(5, 5)
  self.people = { self.hero }
  self.__TODO = false
  return self
end

function World:__tostring()
  return '[World w:' .. self.w .. ' h:' .. self.h .. ']'
end

--
-- one step in the world
--
function World:step()
  for _,person in ipairs(self.people) do
    person:step()
  end
end

--
-- return the stack of tiles (max 10)
--
function World:tile_stack(x, y)
  if x < 0 or y < 0 or x > (self.w-1) or y > (self.h-1) then
    return { Block.NOTHING }
  elseif x == 2 and y == 2 then
    return { Block.GRASS, Block.WOODEN_WALL }
  else
    return { Block.GRASS }
  end
end

-- 
-- return a list of people among the tiles
--
function World:people_in_area(x1, y1, x2, y2)
  local cond = function(p) return (p.x >= x1 and p.x < x2 and p.y >= y1 and p.y < y2) end
  return funct.filter(self.people, cond)
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
function World:tile_walkable(x, y)
  local st = self:tile_stack(x, y)
  if st[1] == Block.WATER or st[1] == Block.NOTHING then
    return false
  end
  return true
end

world = World:new(30, 30) -- global!!
--return world

-- vim: ts=2:sw=2:sts=2:expandtab
