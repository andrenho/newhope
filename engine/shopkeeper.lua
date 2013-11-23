local Shopkeeper = setmetatable(Person, {})
Shopkeeper.__index = Shopkeeper

function Shopkeeper:new(x, y)
  local self = setmetatable({}, Shopkeeper)
  self:__init(x, y)
  return self
end

function Shopkeeper:type()
  return 'Shopkeeper'
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

function Shopkeeper:__respond(person, message, parameters)
  return Shopkeeper.Talk.GREETING, {}
end


return Shopkeeper

-- vim: ts=2:sw=2:sts=2:expandtab
