local StaticObject = setmetatable({}, require('phys.object'))
StaticObject.__index = StaticObject

function StaticObject:new(x, y, w, h)
   local self = setmetatable({}, StaticObject)
   self.body = love.physics.newBody(physics.pworld, x, y)
   self.shape = love.physics.newRectangleShape(w, h)
   self.fixture = love.physics.newFixture(self.body, self.shape)
   return self
end

function StaticObject:draw_wireframe()
   love.graphics.setColor(128, 0, 0)
   local pts = table.pack(self.body:getWorldPoints(self.shape:getPoints()))
   for k, pt in ipairs(pts) do pts[k] = pt * 10 end
   love.graphics.polygon('line', unpack(pts))
end

-------------
-- PRIVATE --
-------------

function StaticObject:__tostring()
   return '[StaticObject]'
end

function StaticObject:type() return 'StaticObject' end

return StaticObject

-- vim: ts=3:sw=3:sts=3:expandtab
