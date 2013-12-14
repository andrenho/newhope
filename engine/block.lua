local Block = {}
Block.__index = Block
Block.all = {}

local block_count = 0
function Block:__new(description, solid, friction)
  local self = setmetatable({}, Block)
  self.id = block_count
  block_count = block_count + 1
  self.description = description
  if solid == nil then
    self.solid = true
  else
    self.solid = solid
  end
  self.friction = friction or 1
  return self
end

function Block.add(description, solid, friction)
  Block[description] = Block:__new(description, solid, friction)
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

--        name            solid   friction
Block.add('NOTHING',      false,  0)
Block.add('GRASS',        true,   1)
Block.add('WATER',        false,  10)
Block.add('WOODEN_WALL',  true)
Block.add('FLOOR',        true)
Block.add('DOOR_OPEN',    false)
Block.add('DOOR_CLOSED',  true)

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
