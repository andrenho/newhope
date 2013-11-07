local block = require('block')

local world = {}

function world:initialize(w, h)
  self.w = w
  self.h = h
end

-- return the stack of tiles (max 10)
function world:tile_stack(x, y)
  if x == 2 and y == 2 then
    return { block.WATER }
  else
    return { block.GRASS }
  end
end

return world

-- vim: ts=2:sw=2:sts=2:expandtab
