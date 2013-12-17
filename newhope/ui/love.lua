local ui = {
   mode = 'wireframe',
}

function love.load() 
end

function love.update(dt) 
end

function love.draw()
end
   --[[
   love.graphics.setColor(72, 160, 14)
   love.graphics.polygon('fill', physics.objects[1].body:getWorldPoints(
      physics.objects[1].shape:getPoints()))
      ]]

function love.mousepressed(x, y, button) 
end

function love.mousereleased(x, y, button) 
end

function love.keypressed(key, unicode) 
end

function love.keyreleased(key, unicode) 
end

function love.focus(f) 
end

function love.quit() 
end

-------------
-- PRIVATE --
-------------

local function __draw_wireframe()
end

-- vim: ts=3:sw=3:sts=3:expandtab
