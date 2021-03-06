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
   return self:__pos()
end

-------------
-- PRIVATE --
-------------

function Object:__init(x, y)
   self.init_x, self.init_y = x, y
   self.is_vehicle = false
   self.is_npc = false
   return self
end

function Object:__tostring()
   return '[Object]'
end

return Object

-- vim: ts=2:sw=2:sts=2:expandtab
