local Game = {}
Game.__index = Game

world = nil -- global variable world

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

function Game:new(w, callbacks)
  assert(w:type() == 'World')
  world = w
  local self = setmetatable({}, Game)
  self.callbacks = self:__check_callbacks(callbacks)
  return self
end

function Game:start()
  self.callbacks.initialize_ui()
  self.callbacks.add_dynamic_objects(world.dynamic_objects)
  while self.callbacks.active() do
    -- get current time
    local nxt = self.callbacks.current_time_ms() + 1000/60
    -- advance frame
    local collisions = self.callbacks.do_physics(world.dynamic_objects)
    world:step(collisions)
    -- draw screen
    local objects = world:objects_in_area(self.callbacks.window_tiles())
    self.callbacks.render(objects)
    -- run commands
    self:__execute_commands(self.callbacks.get_commands())
    -- wait, if necessary
    if self.callbacks.current_time_ms() < nxt then 
      self.callbacks.sleep_ms(nxt - self.callbacks.current_time_ms()) 
    end
  end
  self.callbacks.finish_ui()
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
  --self.callbacks.reset_forces(world.player)
  local speed = 8
  local x, y = 0, 0
  if cmd.up then y = -speed elseif cmd.down then y = speed end
  if cmd.left then x = -speed elseif cmd.right then x = speed end
  self.callbacks.set_velocity(world.player, x, y)
end

function Game:__tostring()
  return '[Game]'
end

return Game

-- vim: ts=2:sw=2:sts=2:expandtab
