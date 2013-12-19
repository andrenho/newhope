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
game   = my_require('game')

-- load classes
World         = my_require('world')
-- map --
Block         = my_require('block')
City          = my_require('city')
Building      = my_require('building')
BuildingType  = my_require('buildingtype')
-- objects --
Object        = my_require('object')
StaticPerson  = my_require('staticperson')
DynamicPerson = my_require('dynamicperson')
Player        = my_require('player')
CarModel      = my_require('carmodel')
Car           = my_require('car')

-- create game
world = World:new()
ui    = game.UI:new()
game  = game.Game:new()

-- vim: ts=3:sw=3:sts=3:expandtab