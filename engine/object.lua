local Object = {}
Object.__index = Object

function Object:type()
   return 'Object'
end

----------------
--   ABSTRACT   --
----------------

function Object:step()
end

function Object:collision(other)
end

function Object:init_physics()
   error('C override')
end

function Object:pos()
   error('C override')
end

-------------
-- PRIVATE --
-------------

function Object:__init(x, y)
   self.x, self.y = x, y
   self.is_vehicle = false
   return self
end

function Object:__tostring()
   return '[Object]'
end

return Object

-- vim: ts=2:sw=2:sts=2:expandtab
