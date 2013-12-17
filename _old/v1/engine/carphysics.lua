local CarPhysics = {}
CarPhysics.__index = CarPhysics

function CarPhysics:new(controls, attrib)
  local self = setmetatable({}, CarPhysics)
  self.controls = controls
  self.attrib = attrib
  self.direction = 0
  self.speed = 0
  return self
end

function CarPhysics:next_frame()
  -- positive longitudinal forces
  local engine = self:__engine_force()
  local inertia = self:__inertia()
  
  -- negative longitudinal forces
  local drag = self:__drag()
  local friction = self:__friction()

  -- calculate front wheel
  local front_wheel = engine + inertia + drag + friction
  local rear_wheel = friction

  return front_wheel + rear_wheel
end

-------------
-- PRIVATE --
-------------

function CarPhysics:__engine_force()
  return geo.Vector:new(0, 0)
end

function CarPhysics:__inertia()
  return geo.Vector:new(0, 0)
end

function CarPhysics:__drag()
  return geo.Vector:new(0, 0)
end

function CarPhysics:__friction()
  return geo.Vector:new(0, 0)
end

function CarPhysics:__tostring()
  return '[CarPhysics]'
end

return CarPhysics

-- vim: ts=2:sw=2:sts=2:expandtab
