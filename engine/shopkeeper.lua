local Shopkeeper = setmetatable(Person, {})
Shopkeeper.__index = Shopkeeper

function Shopkeeper:new(x, y)
  local self = setmetatable({}, Shopkeeper)
  self:__init(x, y)
  return self
end

function Shopkeeper:step()
  -- TODO
end

-------------
-- PRIVATE --
-------------

function Shopkeeper:__tostring()
  return '[Shopkeeper]'
end

return Shopkeeper

-- vim: ts=2:sw=2:sts=2:expandtab
