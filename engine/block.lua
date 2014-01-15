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
Block.add('GRASS',        { 98, 153, 112},  true,   1)
Block.add('SNOW',         {248, 248, 248},  true,   1)
Block.add('TUNDRA',       {193, 193, 180},  true,   1)
Block.add('BARE',         {130, 130, 130},  true,   1)
Block.add('TEMPFOR',      {112, 166,  98},  true,   1)
Block.add('SAVANNAH',     {153, 180, 112},  true,   1)
Block.add('TROPFOR',      { 85, 139, 112},  true,   1)
Block.add('DESERT',       {212, 192, 155},  true,   1)
Block.add('BEACH',        {172, 159, 139},  true,   1)
Block.add('SHRUBLAND',    {196, 204 ,187},  true,   1)
Block.add('WATER',        { 54,  54,  97},  false,  10)
Block.add('WOODEN_WALL',  {0x00,0x00,0x00}, true)
Block.add('FLOOR',        {0x80,0x00,0x00}, true)
Block.add('DOOR_OPEN',    {0x00,0x00,0x00}, false)
Block.add('DOOR_CLOSED',  {0x00,0x00,0x00}, true)

--Block.TROPFOR.color = { 0, 0, 0 }

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
