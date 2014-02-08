local Medic = setmetatable({}, ai.NPC)
Medic.__index = Medic

function Medic:new(x, y)
   return ai.NPC.new(self, x, y)
end

-------------
-- PRIVATE --
-------------

function Medic:__tostring()
   return '[Medic]'
end

function Medic:type() return 'Medic' end

return Medic

-- vim: ts=3:sw=3:sts=3:expandtab
