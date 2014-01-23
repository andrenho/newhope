local Bartender = setmetatable({}, ai.NPC)
Bartender.__index = Bartender

function Bartender:new(x, y)
   return ai.NPC.new(self, x, y)
end

-------------
-- PRIVATE --
-------------

function Bartender:__tostring()
   return '[Bartender]'
end

function Bartender:type() return 'Bartender' end

return Bartender

-- vim: ts=3:sw=3:sts=3:expandtab
