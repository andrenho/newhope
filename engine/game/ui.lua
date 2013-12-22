local UI = {}
UI.__index = UI

function UI:new()
   local self = setmetatable({}, UI)
   return self
end

function UI:initialize()
   self.c_ptr = self:__c_init()
end

function UI:message(text, person, type_, options)
   local tp = type_ or 'message'
   assert(type(text) == 'string')
   if tp == 'message' then
      local pos = nil 
      if person then pos = person:pos() end
      self:__message(text, pos)
   end
end

function UI:__message(text, person_pos, response_type, options)
   error('C function')
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
