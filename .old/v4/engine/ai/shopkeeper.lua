local Shopkeeper = setmetatable({}, ai.NPC)
Shopkeeper.__index = Shopkeeper

function Shopkeeper:new(x, y)
   return ai.NPC.new(self, x, y)
end

function Shopkeeper:talk_to_player(message)
   ui:message(_t('Take a look at our fine merchanisings!'), self)
   ui:buy_sell(self.city)
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
