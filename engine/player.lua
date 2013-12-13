local Player = setmetatable({}, Person)
Player.__index = Player

--
-- Create a new hero (player)
--
function Player:new(x, y)
  local self = setmetatable({}, Player)
  Person.__init(self, x, y)
  self.__speed = 8
  return self
end


function Player:type()
  return 'Player'
end


-------------
-- PRIVATE --
-------------

function Player:__tostring()
  return '[Player x:' .. self.x .. ' y:' .. self.y .. ']'
end

return Player

-- vim: ts=2:sw=2:sts=2:expandtab
