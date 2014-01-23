local NPC = setmetatable({}, Person)
NPC.__index = NPC

function NPC:new(x, y)
   local self = setmetatable({}, self:__init(x, y))
   self.is_npc = true
   return self
end

function NPC:respond_to(other, message)
   return 'BYE', 'Get out.', {}
end

function NPC:act()
end

-------------
-- PRIVATE --
-------------

function NPC:__tostring()
   return '[NPC]'
end

function NPC:type() return 'NPC' end

return NPC

-- vim: ts=3:sw=3:sts=3:expandtab
