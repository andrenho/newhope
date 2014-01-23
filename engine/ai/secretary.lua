local Secretary = setmetatable({}, ai.NPC)
Secretary.__index = Secretary

function Secretary:new(x, y)
   return ai.NPC.new(self, x, y)
end

-------------
-- PRIVATE --
-------------

function Secretary:__tostring()
   return '[Secretary]'
end

function Secretary:type() return 'Secretary' end

return Secretary

-- vim: ts=3:sw=3:sts=3:expandtab
