local Block  = require('block')
local Person = require('person')
local funct  = require('util/funct')

local World = {}
World.__index = World

--
-- create a new world map
-- 
function World:new(w, h)
  local self = setmetatable({}, World)
  self.w = w
  self.h = h
  self.hero = Person:new(50, 50)
  self.people = { self.hero }
  return self
end

function World:__tostring()
  return '[World w:' .. self.w .. ' h:' .. self.h .. ']'
end

--
-- one step in the world
--
function World:step()
  self.hero.x = self.hero.x + 0.1
end

--
-- return the stack of tiles (max 10)
--
function World:tile_stack(x, y)
  if x < 0 or y < 0 or x > (self.w-1) or y > (self.h-1) then
    return { Block.NOTHING }
  elseif x == 2 and y == 2 then
    return { Block.WATER }
  else
    return { Block.GRASS }
  end
end

-- 
-- return a list of people among the tiles
--
function World:people_in_area(x1, y1, x2, y2)
  cond = function(p) return (p.x >= x1 and p.x < x2 and p.y >= y1 and p.y < y2) end
  return funct.filter(self.people, cond)
end

return World:new(300, 300)

-- vim: ts=2:sw=2:sts=2:expandtab
