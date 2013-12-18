-- most of this code was taken from
-- http://www.iforce2d.net/b2dtut/top-down-car

local WheelObject = setmetatable({}, require('phys.object'))
WheelObject.__index = WheelObject

function WheelObject:new()
   local self = setmetatable({}, WheelObject)
   self.body = love.physics.newBody(physics.pworld, 0, 0, 'dynamic')
   self.shape = love.physics.newRectangleShape(0.5, 1.25)
   self.fixture = love.physics.newFixture(self.body, self.shape, 1)
   self.color = { 0, 128, 0 }
   return self
end

function WheelObject:lateral_velocity()
   local current_right_normal = vector.new(self.body:getWorldVector(1, 0))
   return (current_right_normal * vector.new(self.body:getLinearVelocity())) * current_right_normal
end

function WheelObject:forward_velocity()
   local current_forward_normal = vector.new(self.body:getWorldVector(0, 1))
   return (current_forward_normal * vector.new(self.body:getLinearVelocity())) * current_forward_normal
end

function WheelObject:update_friction()
   -- lateral linear velocity
   local max_lateral_impulse = 0.008
   local impulse = -self:lateral_velocity() * self.body:getMass()
   if impulse:len() > max_lateral_impulse then
      impulse = impulse * max_lateral_impulse / impulse:len()
   end
   local center_x, center_y = self.body:getWorldCenter()
   self.body:applyLinearImpulse(impulse.x, impulse.y, center_x, center_y)

   -- angular velocity
   self.body:applyAngularImpulse(0.1 * self.body:getInertia() * -self.body:getAngularVelocity())

   -- forward linear velocity
   local current_forward_normal = self:forward_velocity():normalized()
   local current_forward_speed = current_forward_normal:len()
   local drag_force_magnitude = -0.1 * current_forward_speed
   local drag_normal = current_forward_normal * drag_force_magnitude
   self.body:applyForce(drag_normal.x, drag_normal.y, center_x, center_y)
end

function WheelObject:update_drive(controls)
   local max_forward_speed  = 250
   local max_backward_speed = -40
   local max_drive_force    = 3

   -- find desired speed
   local desired_speed = 0
   if controls.accelerate then
      desired_speed = max_forward_speed
   elseif controls.breaks then
      desired_speed = max_backward_speed
   else
      return
   end

   -- find speed in forward direction
   local current_forward_normal = vector.new(self.body:getWorldVector(0, 1))
   local current_speed = self:forward_velocity() * current_forward_normal

   -- apply necessary force
   local force = 0
   if desired_speed > current_speed then
      force = max_drive_force
   elseif desired_speed < current_speed then
      force = -max_drive_force
   else
      return
   end

   local force_normal = current_forward_normal * force
   self.body:applyForce(force_normal.x, force_normal.y, self.body:getWorldCenter())
end


function WheelObject:update_turn(controls)
   -- update torque
   local desired_torque = 0
   if controls.right > 0 then
      desired_torque = 0.01
   elseif controls.left > 0 then
      desired_torque = -0.01
   end
   self.body:applyTorque(desired_torque)
end


function WheelObject:draw_wireframe()
   love.graphics.setColor(unpack(self.color))
   local pts = table.pack(self.body:getWorldPoints(self.shape:getPoints()))
   for k, pt in ipairs(pts) do pts[k] = pt * 10 end
   love.graphics.polygon('line', unpack(pts))
end

-------------
-- PRIVATE --
-------------

function WheelObject:__tostring()
   return '[WheelObject]'
end

function WheelObject:type() return 'WheelObject' end

return WheelObject

-- vim: ts=3:sw=3:sts=3:expandtab
