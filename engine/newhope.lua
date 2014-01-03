require('util.strict') -- don't allow globals to be declared inside functions

local loaded = {}
function my_require(module)
   if loaded[module] then
      package.loaded[module] = nil
   end
   loaded[module] = true
   return require(module)
end

-- load modules
table   = my_require('util.table')
funct   = my_require('util.funct')
string  = my_require('util.string')
voronoi = my_require('util.voronoi')
game    = my_require('game')

-- load classes
Block          = my_require('block')
World          = my_require('world')
-- map --
MapGen         = my_require('mapgen')
Building       = my_require('building')
BuildingLayout = my_require('buildinglayout')
City           = my_require('city')
CityLayout     = my_require('citylayout')
-- objects --
Object         = my_require('object')
Person         = my_require('person')
Player         = my_require('player')
VehicleModel   = my_require('vehiclemodel')
Vehicle        = my_require('vehicle')

-- AI module
ai = my_require('ai')

-- create game
world = World:new(-10000, -10000, 10000, 10000)
ui    = game.UI:new()
game  = game.Game:new()

-- vim: ts=3:sw=3:sts=3:expandtab
