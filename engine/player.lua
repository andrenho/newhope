local Player = setmetatable({}, Person)
Player.__index = Player

--
-- Create a new hero (player)
--
function Player:new(x, y)
  local self = setmetatable({}, Player)
  self:__init(x, y)
  self.image = 0
  return self
end


-------------
-- PRIVATE --
-------------

function Player:__tostring()
  return '[Player x:' .. self.x .. ' y:' .. self.y .. ']'
end

--
-- Move the unit.
--
function Player:__move()
  local px, py = self.x, self.y
  local r = Person.__move(self)

  -- check if there's a unit to interact in that position
  if r then
    local p = world:person_in_position(self.x, self.y, self)
    if p then
      self:__interact_with(p)
      self.x, self.y = px, py -- don't move
      return false
    end
  end

  return r
end
 


--
-- Interact with another unit
--
Player.Talk = {
  GREETING = 'Hi.',
}

function Player:__interact_with(p)
  print(msg.msgbox 'Hello')
  --[[
  if not world.talk_queue:peek_from(self) then
    world.talk_queue:enqueue(self, p, Player.Talk.GREETING)
  end
  ]]
end

--
-- Respond to interaction
--
function Player:__respond_to_interaction(p)
end

return Player

-- vim: ts=2:sw=2:sts=2:expandtab
