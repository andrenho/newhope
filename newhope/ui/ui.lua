local UI = {}
UI.__index = UI

function UI:new()
   local self = setmetatable({}, UI)
   self.mode = 'wireframe'
   return self
end

function UI:draw()
   -- draw wireframe (physics objects)
   if self.mode == 'wireframe' then
      love.graphics.setBackgroundColor(255, 255, 255)
      love.graphics.clear()
      for _,obj in ipairs(physics.objects) do
         obj:draw_wireframe()
      end
   end
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
