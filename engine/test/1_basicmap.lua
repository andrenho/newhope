-- 
-- Chunk 1: basic map construction
--

function test_NoEmptyBlock()
  for x = 0,20 do
    for y = 0,20 do
      assert(#world:tiles(x, y) >= 1)
    end
  end
end

function test_InvalidBlockReturnWater()
  assert(world:tiles(-1, -1)[1] == Block.WATER)
  assert(world:tiles(10000, 10000)[1] == Block.WATER)
end

function test_PredefinedBlock()
  world.predefined_tiles[world:unique_tile_id(10, 10)] = { Block.GRASS, Block.GRASS }
  assert(world:tiles(10, 10)[2] == Block.GRASS)
end

-- vim: ts=2:sw=2:sts=2:expandtab
