local Person = setmetatable({}, Object)
Person.__index = Person

function Person:new(x, y)
   return Object.__init(self, x, y)
end

function Person:set_position(x, y)
  error('C override')
end

function Person:set_target(x, y)
  error('C override')
end

-------------
-- PRIVATE --
-------------

function Person:__tostring()
   return '[Person]'
end

function Person:type() return 'Person' end

return Person

-- vim: ts=3:sw=3:sts=3:expandtab
