--
-- Chunk 2: person moving around
--

function test_PersonMove()
  local p = world.player
  -- don't move
  p.x, p.y = 10.0, 10.0
  local ox, oy = p.x, p.y
  world:step()
  assert(p.x == ox and p.y == oy)
  -- speed 1
  p:change_speed(1)
  p:turn_to(0)
  world:step()
  assert(p.x > ox)
  local dif = p.x - ox
  -- speed 2
  p:change_speed(2)
  local ox2 = p.x
  world:step()
  assert(dif < (p.x - ox2))
end


function test_PersonBlocking()
  world.predefined_tiles[world:unique_tile_id(11, 10)] = { Block.GRASS, Block.GRASS, Block.GRASS }
  local p = world.player
  p.x, p.y = 10.0, 10.0
  p:change_speed(2)
  p:turn_to(0)
  for _=1,20 do world:step() end
  assert(p.x < 11)
end

-- vim: ts=2:sw=2:sts=2:expandtab
