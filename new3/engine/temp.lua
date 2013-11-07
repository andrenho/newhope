local world = require("world")

world:initialize(10, 20)
print(world:tile_stack(0, 0)[1])
