local StaticPerson = setmetatable({}, Object)
StaticPerson.__index = StaticPerson

function StaticPerson:new(x, y)
  local self = setmetatable({}, StaticPerson)
  self:__init(x,y)
  return self
end

function StaticPerson:create_physics_body()
  error('This method must be implemented in C.')
end

function StaticPerson:type()
  return 'StaticPerson'
end

-------------
-- PRIVATE --
-------------

function StaticPerson:__tostring()
  return '[StaticPerson]'
end

return StaticPerson

-- vim: ts=2:sw=2:sts=2:expandtab
