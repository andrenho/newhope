local Player = setmetatable({}, Person)
Player.__index = Player

--
-- Create a new hero (player)
--
function Player:new(x, y)
  local self = setmetatable({}, Player)
  Person.__init(self, x, y)
  self:__setup_collision_handler()
  self.__speed = 8
  return self
end


function Player:type()
  return 'Player'
end


function Player:collision(against)
  print('Collision against '..tostring(against))
end


-------------
-- PRIVATE --
-------------

function Player:__tostring()
  return '[Player x:' .. self.x .. ' y:' .. self.y .. ']'
end

function Player:__setup_collision_handler()
  error('This function should have been replaced by a C function.')
end

return Player

-- vim: ts=2:sw=2:sts=2:expandtab
