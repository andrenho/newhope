local PhysicsEngine = {}
PhysicsEngine.__index = PhysicsEngine

function PhysicsEngine:new()
  local self = setmetatable({}, PhysicsEngine)
  return self
end

function PhysicsEngine:type()
  return 'PhysicsEngine'
end

-------------
-- PRIVATE --
-------------

function PhysicsEngine:__tostring()
  return '[PhysicsEngine]'
end

return PhysicsEngine

-- vim: ts=2:sw=2:sts=2:expandtab
