local CarObject = setmetatable({}, require('phys.object'))
CarObject.__index = CarObject

function CarObject:new(world_object, x, y, attrib, color)
   local self = setmetatable({}, CarObject)
   self.color = color or { 0, 128, 0 }
   -- car body
   self.body = love.physics.newBody(physics.pworld, x, y)
   self.shape = love.physics.newPolygonShape(
         1.5,    0,
           3,  2.5,
         2.8,  5.5,
           1,   10,
          -1,   10,
        -2.8,  5.5,
          -3,  2.5,
        -1.5,    0)
   self.fixture = love.physics.newFixture(self.body, self.shape, 1)
   self.fixture:setUserData(world_object)
   -- wheels
   self.wheels = {}
   self.joints = {}
   local wheel_pos = { { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 } }
   for i = 1,4 do
      self.wheels[i] = phys.WheelObject:new()
   end
   self.joints[1] = love.physics.newRevoluteJoint(self.wheels[1].body,
            self.body, -3, 0.75, true)
   self.joints[2] = love.physics.newRevoluteJoint(self.wheels[2].body,
            self.body,3, 0.75, true)
   self.joints[3] = love.physics.newRevoluteJoint(self.wheels[3].body, 
            self.body, -3, 8.5, true)
   self.joints[4] = love.physics.newRevoluteJoint(self.wheels[4].body, 
            self.body, 3, 8.5, true)
   for i = 1,4 do
      --self.joints[i]:setLimitsEnabled(true)
      --self.joints[i]:setLimits(0, 0)
   end
   return self
end

function CarObject:draw_wireframe()
   for _,wheel in ipairs(self.wheels) do
      wheel:draw_wireframe()
   end
   love.graphics.setColor(unpack(self.color))
   local pts = table.pack(self.body:getWorldPoints(self.shape:getPoints()))
   for k, pt in ipairs(pts) do pts[k] = pt * 10 end
   love.graphics.polygon('line', unpack(pts))
end

function CarObject:update(controls)
   for _,wheel in ipairs(self.wheels) do
      wheel:update_friction()
   end
   for _,wheel in ipairs(self.wheels) do
      wheel:update_drive(controls)
      --wheel:update_turn(controls)
   end
end

-------------
-- PRIVATE --
-------------

function CarObject:__tostring()
   return '[CarObject]'
end

function CarObject:type() return 'CarObject' end

return CarObject

-- vim: ts=3:sw=3:sts=3:expandtab
