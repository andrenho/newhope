local Object = {}
Object.__index = Object

function Object:type()
  return 'Object'
end

----------------
--  ABSTRACT  --
----------------

function Object:step()
end

function Object:create_physics_body()
  error('Abstract method')
end

-------------
-- PRIVATE --
-------------

function Object:__init(x, y)
  self.x, self.y = x, y
  self.body = 0
  return self
end

function Object:__tostring()
  return '[Object]'
end

return Object

-- vim: ts=2:sw=2:sts=2:expandtab
