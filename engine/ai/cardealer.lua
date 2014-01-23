local CarDealer = setmetatable({}, ai.NPC)
CarDealer.__index = CarDealer

function CarDealer:new(x, y)
   return ai.NPC.new(self, x, y)
end

-------------
-- PRIVATE --
-------------

function CarDealer:__tostring()
   return '[CarDealer]'
end

function CarDealer:type() return 'CarDealer' end

return CarDealer

-- vim: ts=3:sw=3:sts=3:expandtab
