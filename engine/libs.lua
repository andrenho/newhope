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
geo  = my_require('geo')
game = my_require('game')

-- classes
Block         = my_require('block')
World         = my_require('world')
DynamicObject = my_require('dynamicobject')
Person        = my_require('person')
Player        = my_require('player')
Shopkeeper    = my_require('shopkeeper')
City          = my_require('city')
Building      = my_require('building')
BuildingType  = my_require('buildingtype')

-- vim: ts=2:sw=2:sts=2:expandtab
