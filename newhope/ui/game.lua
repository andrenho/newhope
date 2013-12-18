local Game = {}
Game.__index = Game

function Game:new()
   local self = setmetatable({}, Game)
   return self
end

function Game:keypressed(key, unicode)
   if key == 'q' then
      os.exit()
   elseif key == 'up' then
      world.player:up()
   end
end

function Game:step(dt)
   world.player:up()
   physics.pworld:update(dt)
end

-------------
-- PRIVATE --
-------------

function Game:__tostring()
   return '[Game]'
end

function Game:type() return 'Game' end

return Game

-- vim: ts=3:sw=3:sts=3:expandtab