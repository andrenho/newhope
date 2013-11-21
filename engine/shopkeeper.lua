local Shopkeeper = setmetatable(Person, {})
Shopkeeper.__index = Shopkeeper

function Shopkeeper:new(x, y)
  local self = setmetatable({}, Shopkeeper)
  self:__init(x, y)
  return self
end

-------------
-- PRIVATE --
-------------

function Shopkeeper:__tostring()
  return '[Shopkeeper]'
end

--
-- Interaction with player
--
Shopkeeper.Talk = {
  GREETING = "Welcome sir, please take a look at our merchandises.",
}

function Shopkeeper:__respond_to_interaction()
  --[[
  local question = world.talk_queue:dequeue(self)
  if question ~= nil then
    if question.message == Player.Talk.GREETING then
      world.talk_queue:enqueue(self, question.from, Shopkeeper.Talk.GREETING, {})
    else
      error('Don\'t know how to respond to message: ', tostring(question))
    end
  end
  return self
  ]]
end


return Shopkeeper

-- vim: ts=2:sw=2:sts=2:expandtab
