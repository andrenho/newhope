local Game = {}
Game.__index = Game

Game.__required_callbacks = {
  'active',
  'current_time_ms',
  'finish_ui',
  'get_commands',
  'initialize_ui',
  'sleep_ms',
  'render',
  'window_tiles',
}

Game.__physics_callbacks = {
  'add_dynamic_object',
  'apply_force',
  'step',
  'setup_player_collision_handler',
  'set_velocity',
  'reset_forces',
}

function Game:new(w, cb)
  assert(w:type() == 'World')
  local self = setmetatable({}, Game)
  self.__callbacks = self:__check_callbacks(cb)

  -- setup globals
  physics = self:__create_physics_object(cb)
  world = w

  return self
end

function Game:start()
  self.__callbacks.initialize_ui()
  world:initialize()
  while self.__callbacks.active() do
    -- get current time
    local nxt = self.__callbacks.current_time_ms() + 1000/60
    -- advance frame
    local collisions = physics.step(world.dynamic_objects)
    world:step(collisions)
    -- draw screen
    local objects = world:objects_in_area(self.__callbacks.window_tiles())
    self.__callbacks.render(objects)
    -- run commands
    self:__execute_commands(self.__callbacks.get_commands())
    -- wait, if necessary
    if self.__callbacks.current_time_ms() < nxt then 
      self.__callbacks.sleep_ms(nxt - self.__callbacks.current_time_ms()) 
    end
  end
  self.__callbacks.finish_ui()
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
  world.player:set_movement_vector(x, y)
end

function Game:__create_physics_object(cb)
  local missing = {}
  for _,cb_name in ipairs(Game.__physics_callbacks) do
    if not cb[cb_name] then missing[#missing+1] = cb_name end
  end
  if #missing > 0 then error('Physics callbacks missing: '..table.concat(missing, ', '), 2) end
  return cb
end

function Game:__tostring()
  return '[Game]'
end

return Game

-- vim: ts=2:sw=2:sts=2:expandtab
