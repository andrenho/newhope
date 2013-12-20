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
      self:__check_keyboard_state(ui:keyboard_state())
      for _,evt in ipairs(ui:user_events()) do
         self:__do_event(evt)
      end
      
      -- redraw screen
      local x1,y1,x2,y2 = ui:visible_tiles()
      ui:render(world:objects_in_area(x1,y1,x2,y2), x1, y1, x2, y2)

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

function Game:__check_keyboard_state(st)
   local vehicle = world.player:in_vehicle()
   if vehicle then
      vehicle.controls.accelerate = st.up
      vehicle.controls.breaks = st.down
      if st.right then 
         vehicle.controls.steering = 1
      elseif st.left then
         vehicle.controls.steering = -1
      else
         vehicle.controls.steering = 0
      end
   else
      local pos = world.player:pos()
      if st.up then pos.y = pos.y-100 elseif st.down then pos.y = pos.y+100 end
      if st.right then pos.x = pos.x+100 elseif st.left then pos.x = pos.x-100 end
      world.player:set_target(pos.x, pos.y)
   end
end

function Game:__do_event(e)
   if e.event == 'quit' then
      self.active = false
   elseif e.event == 'key' then
      if e.key == 'E' and world.player:in_vehicle() then
         world.player:exit_vehicle()
      end
   end
end

function Game:__tostring()
   return '[Game]'
end

function Game:type() return 'Game' end

return Game

-- vim: ts=3:sw=3:sts=3:expandtab
