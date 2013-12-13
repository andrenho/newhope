local Block = {}
Block.__index = Block
Block.all = {}

local block_count = 0
function Block:__new(description, friction, immaterial)
  local self = setmetatable({}, Block)
  self.id = block_count
  block_count = block_count + 1
  self.description = description
  self.friction = friction or math.huge
  self.immaterial = immaterial or false
  return self
end

function Block.add(description, friction, immaterial)
  Block[description] = Block:__new(description, friction, immaterial)
  Block.all[#Block.all+1] = Block[description]
end

-------------
-- PRIVATE --
-------------

function Block:__tostring()
  return '['..self.description..']'
end

--------------------
-- LIST OF BLOCKS --
--------------------

Block.add('NOTHING', 0, true)
Block.add('GRASS', 1)
Block.add('WATER')
Block.add('WOODEN_WALL')
Block.add('FLOOR', 1, true)
Block.add('DOOR_OPEN', 0, true)
Block.add('DOOR_CLOSED')

-----------------------
-- GENERATE C HEADER --
-----------------------

function block_output_c()
  print('#ifndef BLOCKS_H')
  print('#define BLOCKS_H')
  print()
  for _,b in pairs(Block.all) do
    print('#define '..b.description..' '..b.id)
  end
  print()
  print('#endif')
end

return Block

-- vim: ts=2:sw=2:sts=2:expandtab
