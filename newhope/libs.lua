-- the C program that'll call this module should set package.path accordingly

local loaded = {}
function my_require(module)
  if loaded[module] then
    package.loaded[module] = nil
  end
  loaded[module] = true
  return require(module)
end

-- require 'util.strict' -- forbids declaring globals in functions

-- modules
table  = my_require('util.table')
funct  = my_require('util.funct')
string = my_require('util.string')

-- libraries
game = my_require('game')

-- classes
World         = my_require('world')

-- classes (map)
Block         = my_require('block')
City          = my_require('city')
Building      = my_require('building')
BuildingType  = my_require('buildingtype')

-- classes (objects)
Object        = my_require('object')
StaticPerson  = my_require('staticperson')
DynamicPerson = my_require('dynamicperson')
Player        = my_require('player')
CarModel      = my_require('carmodel')
Car           = my_require('car')

-- global variables set elsewhere (game.Game:new)
world = nil
physics = nil

-- vim: ts=2:sw=2:sts=2:expandtab
