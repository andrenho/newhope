local DynamicPerson = setmetatable({}, Object)
DynamicPerson.__index = DynamicPerson

function DynamicPerson:new(x, y)
  local self = self or setmetatable({}, DynamicPerson)
  self:__init(x,y)
  physics:add_object(phys.DynamicObject:new(x, y, 0.5))
  return self
end

function DynamicPerson:set_target(x,y)
  error('This method must be implemented in C.')
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
