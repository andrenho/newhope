require('util.strict') -- doesn't allow globals to be declared inside functions

local loaded = {}
function my_require(module)
   if loaded[module] then
      package.loaded[module] = nil
   end
   loaded[module] = true
   return require(module)
end

-- load modules
phys = my_require('phys')

-- load classes
Game = my_require('game')
UI   = my_require('ui.ui')

-- initialize game
world   = nil -- TODO
physics = phys.Physics:new()
game    = Game:new()

-- temporary (TODO)
physics:add_object(phys.StaticObject:new(400, 80, 10, 10))
physics:add_object(phys.DynamicObject:new(80, 80, 5, 5))

-- initialize UI
ui = UI:new()

-- setup LÖVE callbacks
function love.draw() ui:draw() end
function love.update(dt) game:step(dt) end
function love.keypressed(key, unicode) game:keypressed(key, unicode) end

-- vim: ts=3:sw=3:sts=3:expandtab
