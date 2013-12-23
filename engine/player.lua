local Player = setmetatable({}, Person)
Player.__index = Player

function Player:new(x, y)
   local self = self or setmetatable({}, Player)
   Person.new(self, x, y)
   self.vehicle = nil
   self.__in_vehicle = false
   return self
end

function Player:pos()
   if self.__in_vehicle then
      return self.vehicle:pos()
   else
      return Person.pos(self)
   end
end


function Player:collision(other)
   -- if own vehicle, enter in it
   if other == self.vehicle and not self.__in_vehicle then
      self.__in_vehicle = true
      self:set_position(-100000, -100000)
   end
   if other:type() == 'StaticPerson' then 
      local t = [[Each call to strtok() returns a pointer to a null-terminated string containing the next token. This string does not include the delimiting byte. If no more tokens are found, strtok() returns NULL.]]
      ui:message(t)
   end
end


function Player:exit_vehicle()
   -- TODO - check if speed = 0
   local vpos = self.vehicle:pos()
   local ppos = self:__pos()
   local fx = vpos.x + funct.max{self.vehicle.attrib.w/2, self.vehicle.attrib.h/2} + 1
   while not world:tile_walkable(fx, ppos.y) do 
      fx = fx + 1 
      if fx > 5 then return false end
   end
   self.__in_vehicle = false
   self:set_position(fx, vpos.y)
   return true
end


function Player:in_vehicle()
   if self.__in_vehicle then return self.vehicle else return nil end
end


-------------
-- PRIVATE --
-------------

function Player:__tostring()
   return '[Player]'
end

function Player:type()
   return 'Player'
end

return Player

-- vim: ts=3:sw=3:sts=3:expandtab
