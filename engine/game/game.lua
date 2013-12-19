local Game = {}
Game.__index = Game

function Game:new()
   local self = setmetatable({}, Game)
   self.active = true
   return self
end

function Game:start()
   -- initialize things
   world:initialize()
   ui:initialize()

   -- main loop
   while self.active do
      -- initialize timer
      local next_frame = ui:now() + 1000/60

      -- process world
      world:step()

      -- get user events
      for _,evt in ipairs(ui:user_events()) do
         self:__do_event(evt)
      end
      
      -- redraw screen
      local x1,y1,x2,y2 = ui:visible_tiles()
      --ui:redraw(world:objects_in_area(x1,y1,x2,y2), x1, y1, x2, y2)
      ui:render(world.objects, x1, y1, x2, y2)

      -- wait for frame
      local now = ui:now()
      if now < next_frame then
         --print(next_frame - now)
         ui:wait(next_frame - now)
      end
   end
end


function Game:clean_up()
   ui:clean_up()
   world:clean_up()
end

-------------
-- PRIVATE --
-------------

function Game:__do_event(e)
   if e.event == 'quit' then
      self.active = false
   end
end

function Game:__tostring()
   return '[Game]'
end

function Game:type() return 'Game' end

return Game

-- vim: ts=3:sw=3:sts=3:expandtab
