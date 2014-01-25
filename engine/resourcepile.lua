local ResourcePile = setmetatable({}, Object)
ResourcePile.__index = ResourcePile

function ResourcePile:new(x, y, resource, city)
   local self = setmetatable({}, ResourcePile)
   self:__init(x, y)
   self.resource = resource
   self.city = city
   return self
end

-------------
-- PRIVATE --
-------------

function ResourcePile:__tostring()
   return '[ResourcePile]'
end

function ResourcePile:type() return 'ResourcePile' end

return ResourcePile

-- vim: ts=3:sw=3:sts=3:expandtab
