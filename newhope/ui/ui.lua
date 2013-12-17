local UI = {}
UI.__index = UI

function UI:new()
   local self = setmetatable({}, UI)
   return self
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
