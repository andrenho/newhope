local Game = {}
Game.__index = Game

world = nil -- global variable world
callbacks = nil

Game.__required_callbacks = {
  'active',
  'add_dynamic_objects',
  'apply_force',
  'current_time_ms',
  'do_physics',
  'finish_ui',
  'get_commands',
  'initialize_ui',
  'set_velocity',
  'sleep_ms',
  'render',
  'reset_forces',
  'window_tiles',
}

function Game:new(w, cb)
  assert(w:type() == 'World')
  world = w
  local self = setmetatable({}, Game)
  callbacks = self:__check_callbacks(cb)
  return self
end

function Game:start()
  callbacks.initialize_ui()
  callbacks.add_dynamic_objects(world.dynamic_objects)
  while callbacks.active() do
    -- get current time
    local nxt = callbacks.current_time_ms() + 1000/60
    -- advance frame
    local collisions = callbacks.do_physics(world.dynamic_objects)
    world:step(collisions)
    -- draw screen
    local objects = world:objects_in_area(callbacks.window_tiles())
    callbacks.render(objects)
    -- run commands
    self:__execute_commands(callbacks.get_commands())
    -- wait, if necessary
    if callbacks.current_time_ms() < nxt then 
      callbacks.sleep_ms(nxt - callbacks.current_time_ms()) 
    end
  end
  callbacks.finish_ui()
end

function Game:type()
  return 'Game'
end

-------------
-- PRIVATE --
-------------

function Game:__check_callbacks(cb)
  local missing = {}
  for _,cb_name in ipairs(Game.__required_callbacks) do
    if not cb[cb_name] then missing[#missing+1] = cb_name end
  end
  if #missing > 0 then error('Callbacks missing: '..table.concat(missing, ', '), 2) end
  return cb
end

function Game:__execute_commands(cmd)
  x, y = 0, 0
  if cmd.up then y = -1 elseif cmd.down then y = 1 end
  if cmd.left then x = -1 elseif cmd.right then x = 1 end
  world.player:set_direction_vector(x, y)
end

function Game:__tostring()
  return '[Game]'
end

return Game

-- vim: ts=2:sw=2:sts=2:expandtab
