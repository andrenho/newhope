local Object = {}
Object.__index = Object

--------------
-- ABSTRACT --
--------------

function Object:draw_wireframe() error('ABSTRACT') end

-------------
-- PRIVATE --
-------------

function Object:__tostring()
   return '[Object]'
end

function Object:type() return 'Object' end

return Object

-- vim: ts=3:sw=3:sts=3:expandtab
