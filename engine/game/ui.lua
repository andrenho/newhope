local UI = {}
UI.__index = UI

function UI:new()
   local self = setmetatable({}, UI)
   return self
end

function UI:initialize()
   self.c_ptr = self:__c_init()
end

function UI:redraw(objects)
   self:__c_redraw(objects)
end

-------------
-- PRIVATE --
-------------

function UI:__tostring()
   return '[UI]'
end

function UI:type() return 'UI' end

return UI

-- vim: ts=3:sw=3:sts=3:expandtab
