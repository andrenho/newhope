--
-- packages
--

-- OO support
function ABSTRACT() error('Abstract method!', 2) end

-- load modules (globals!!!)
require('util.strict')
mod    = require('util.modules')
funct  = mod.require('util.funct')
table  = mod.require('util.table')
string = mod.require('util.string')
msg    = mod.require('msg')

-- load classes
Block        = mod.require('block')
CarModel     = mod.require('carmodel')
Person       = mod.require('person')
Player       = mod.require('player')
Shopkeeper   = mod.require('shopkeeper')
Car          = mod.require('car')
City         = mod.require('city')
Building     = mod.require('building')
BuildingType = mod.require('buildingtype')
TalkQueue    = mod.require('talkqueue')
Callback     = mod.require('callback')
World        = mod.require('world')

-- instances
callback = Callback:new()
world = nil -- will be set by C
