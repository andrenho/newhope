local DynamicPerson = setmetatable({}, Object)
DynamicPerson.__index = DynamicPerson

function DynamicPerson:new(x, y)
  local self = self or setmetatable({}, DynamicPerson)
  self:__init(x,y)
  return self
end

function DynamicPerson:type()
  return 'DynamicPerson'
end

-------------
-- PRIVATE --
-------------

function DynamicPerson:__tostring()
  return '[DynamicPerson]'
end

return DynamicPerson

-- vim: ts=2:sw=2:sts=2:expandtab
