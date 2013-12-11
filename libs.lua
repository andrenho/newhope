package.path = ';;./?/init.lua;./?.lua;../?/init.lua;../?.lua'

local loaded = {}
function my_require(module)
  if loaded[module] then
    package.loaded[module] = nil
  end
  loaded[module] = true
  return require(module)
end

require 'util.strict' -- forbids declaring globals in functions

-- modules
table  = my_require('util.table')
funct  = my_require('util.funct')
string = my_require('util.string')

-- libraries
geo = my_require('geo')

-- classes

-- vim: ts=2:sw=2:sts=2:expandtab
