local Shopkeeper = setmetatable({}, ai.NPC)
Shopkeeper.__index = Shopkeeper

function Shopkeeper:new(x, y)
   return ai.NPC.new(self, x, y)
end

-------------
-- PRIVATE --
-------------

function Shopkeeper:__tostring()
   return '[Shopkeeper]'
end

function Shopkeeper:type() return 'Shopkeeper' end

return Shopkeeper

-- vim: ts=3:sw=3:sts=3:expandtab
