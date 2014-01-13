local Block = {}
Block.__index = Block
Block.all = {}

local block_count = 0
function Block:__new(description, color, solid, friction)
  local self = setmetatable({}, Block)
  self.id = block_count
  block_count = block_count + 1
  self.color = color
  self.description = description
  if solid == nil then
    self.solid = true
  else
    self.solid = solid
  end
  self.friction = friction or 1
  return self
end

function Block.add(description, color, solid, friction)
  Block[description] = Block:__new(description, color, solid, friction)
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

--        name            color             solid   friction
Block.add('NOTHING',      {0x00,0x00,0x00}, false,  0)
Block.add('GRASS',        {0x00,0x80,0x00}, true,   1)
Block.add('SNOW',         {0xff,0xff,0xff}, true,   1)
Block.add('TUNDRA',       {0xa0,0xa0,0xa0}, true,   1)
Block.add('BARE',         {0x50,0x50,0x50}, true,   1)
Block.add('TEMPFOR',      {0x00,0x50,0x00}, true,   1)
Block.add('SAVANNAH',     {0x80,0x80,0x00}, true,   1)
Block.add('TROPFOR',      {0x00,0xa0,0x00}, true,   1)
Block.add('DESERT',       {0xb0,0xb0,0x00}, true,   1)
Block.add('WATER',        {0x00,0x00,0x80}, false,  10)
Block.add('WOODEN_WALL',  {0x00,0x00,0x00}, true)
Block.add('FLOOR',        {0x80,0x00,0x00}, true)
Block.add('DOOR_OPEN',    {0x00,0x00,0x00}, false)
Block.add('DOOR_CLOSED',  {0x00,0x00,0x00}, true)

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
