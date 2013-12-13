local DynamicObject = {}
DynamicObject.__index = DynamicObject

-- physics model
DynamicObject.NOT_SET           = 0
DynamicObject.MOVEABLE_PERSON   = 1
DynamicObject.IMMOVEABLE_PERSON = 2
DynamicObject.CAR               = 3

function DynamicObject:type()
  return 'DynamicObject'
end

-------------
-- PRIVATE --
-------------

function DynamicObject:__init(x, y, w, h, mass, angle, velocity_limit)
  self.physics_model = DynamicObject.NOT_SET
  self.original_x = x
  self.original_y = y
  self.x, self.y = x, y
  self.w, self.h = w, h
  self.mass = mass
  self.angle = angle or 0
  self.velocity_limit = velocity_limit or 0
  self.body, self.shape = nil, nil  -- use for C pointers
  return self
end

function DynamicObject:__tostring()
  return '[DynamicObject]'
end

return DynamicObject

-- vim: ts=2:sw=2:sts=2:expandtab
