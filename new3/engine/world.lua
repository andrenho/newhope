local block = require('block')

local World = {}
World.__index = World

function World:new(w, h)
  local self = setmetatable({}, World)
  self.w = w
  self.h = h
  return self
end

-- return the stack of tiles (max 10)
function World:tile_stack(x, y)
  if x < 0 or y < 0 or x > (self.w-1) or y > (self.h-1) then
    return { block.NOTHING }
  elseif x == 2 and y == 2 then
    return { block.WATER }
  else
    return { block.GRASS }
  end
end

return World:new(30, 40)

-- vim: ts=2:sw=2:sts=2:expandtab
