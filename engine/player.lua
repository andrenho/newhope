local Player = setmetatable({}, DynamicPerson)
Player.__index = Player

function Player:new(x, y)
  local self = self or setmetatable({}, Player)
  DynamicPerson.new(self, x, y)
  return self
end

function Player:type()
  return 'Player'
end

function Player:up()
  self.physics_object:up()
end

function Player:collision(other)
  print(other)
end

function Player:in_vehicle()
  return world.objects[2]
end

-------------
-- PRIVATE --
-------------

function Player:__tostring()
  return '[Player]'
end

return Player

-- vim: ts=2:sw=2:sts=2:expandtab
