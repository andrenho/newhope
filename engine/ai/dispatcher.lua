local Dispatcher = setmetatable({}, ai.NPC)
Dispatcher.__index = Dispatcher

function Dispatcher:new(x, y)
   return ai.NPC.new(self, x, y)
end

-------------
-- PRIVATE --
-------------

function Dispatcher:__tostring()
   return '[Dispatcher]'
end

function Dispatcher:type() return 'Dispatcher' end

return Dispatcher

-- vim: ts=3:sw=3:sts=3:expandtab
