local Worker = setmetatable(Person, {})
Worker.__index = Worker


function Worker:type()
  return 'Worker'
end

-------------
-- PRIVATE --
-------------


function Worker:__init2(x, y)
  Person.__init(self, x, y)
  self.physics_model = DynamicObject.IMMOVEABLE_PERSON
  self.mass = 1000000
end


function Worker:__tostring()
  return '[Worker]'
end

return Worker

-- vim: ts=2:sw=2:sts=2:expandtab
