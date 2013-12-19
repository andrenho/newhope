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
table  = my_require('util.table')
funct  = my_require('util.funct')
string = my_require('util.string')

-- load classes
World         = my_require('world.world')
-- map --
Block         = my_require('world.block')
City          = my_require('world.city')
Building      = my_require('world.building')
BuildingType  = my_require('world.buildingtype')
-- objects --
Object        = my_require('world.object')
StaticPerson  = my_require('world.staticperson')
DynamicPerson = my_require('world.dynamicperson')
Player        = my_require('world.player')
CarModel      = my_require('world.carmodel')
Car           = my_require('world.car')

-- create game
world = World:new()
--game  = ui.Game:new()
--ui    = ui.UI:new()

-- initialize engine
world:initialize()

-- vim: ts=3:sw=3:sts=3:expandtab
