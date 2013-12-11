-- 
-- Chunk 3: talking
--

function test_Talking()
  local talked = false
  local message = function(msg, tp, options, image, person)
    talked = true
    assert(type(msg) == 'string')
    assert(tp == 1)
    assert(person == world.player:id())
  end
  callback:install("message", message)
  world.people[#world.people+1] = Shopkeeper:new(11.0, 10.0)
  world.player.x = 10.0
  world.player.y = 10.0
  world.player:turn_to(0)
  world.player:change_speed(2)
  for _=0,20 do
    world:step()
  end
end

-- vim: ts=2:sw=2:sts=2:expandtab
