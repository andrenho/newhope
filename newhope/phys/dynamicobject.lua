local DynamicObject = {}
DynamicObject.__index = DynamicObject

function DynamicObject:new(x, y, radius)
   local self = setmetatable({}, DynamicObject)
   self.body = love.physics.newBody(physics.pworld, x, y, 'dynamic')
   self.shape = love.physics.newCircleShape(radius)
   self.fixture = love.physics.newFixture(self.body, self.shape, 1)
   return self
end

function DynamicObject:draw_wireframe()
   love.graphics.setColor(0, 128, 0)
   love.graphics.circle('line', self.body:getX(), self.body:getY(), 
         self.shape:getRadius())
end

-------------
-- PRIVATE --
-------------

function DynamicObject:__tostring()
   return '[DynamicObject]'
end

function DynamicObject:type() return 'DynamicObject' end

return DynamicObject

-- vim: ts=3:sw=3:sts=3:expandtab
