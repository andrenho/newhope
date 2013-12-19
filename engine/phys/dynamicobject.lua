local DynamicObject = {}
DynamicObject.__index = DynamicObject

function DynamicObject:new(x, y, radius)
   local self = setmetatable({}, DynamicObject)
   self.body = love.physics.newBody(physics.pworld, x, y, 'dynamic')
   self.shape = love.physics.newCircleShape(radius)
   self.fixture = love.physics.newFixture(self.body, self.shape, 1)
   self.fixture:setDensity(1)
   self.body:setLinearDamping(100)
   return self
end

function DynamicObject:draw_wireframe()
   love.graphics.setColor(0, 128, 0)
   love.graphics.circle('line', self.body:getX() * 10, self.body:getY() * 10, 
         self.shape:getRadius() * 10)
end

function DynamicObject:up()
   self.body:setLinearVelocity(0, -40)
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
