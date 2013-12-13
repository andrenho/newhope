local Shopkeeper = setmetatable(Worker, {})
Shopkeeper.__index = Shopkeeper

function Shopkeeper:new(x, y)
  local self = setmetatable({}, Shopkeeper)
  self:__init2(x, y)
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

return Shopkeeper

-- vim: ts=2:sw=2:sts=2:expandtab
