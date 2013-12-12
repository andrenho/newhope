local Game = {}
Game.__index = Game

world = nil -- global variable world

Game.__required_callbacks = {
  'active',
  'current_time_us',
  'do_physics',
  'finish_ui',
  'get_commands',
  'initialize_ui',
  'sleep_us',
  'render',
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
  while self.callbacks.active() do
    -- get current time
    local nxt = self.callbacks.current_time_us() + 1/60
    -- advance frame
    local collisions = self.callbacks.do_physics(world.dynamic_objects())
    world:step(collisions)
    -- draw screen
    self.callbacks.render(self:__render_units(self.callbacks.window_tiles))
    -- run commands
    for _,cmd in ipairs(self.callbacks.get_commands()) do
      self:__execute_command(cmd)
    end
    -- wait, if necessary
    if self.callbacks.current_time_us() < nxt then 
      self.callbacks.sleep_us(nxt - self.callbacks.current_time_us()) 
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
--  if #missing > 0 then error('Callbacks missing: '..table.concat(missing, ', '), 2) end
  return cb
end

function Game:__render_units(x, y, w, h)
  return {}
end

function Game:__execute_command(cmd)
  error('Invalid command '..cmd)
end

function Game:__tostring()
  return '[Game]'
end

return Game

-- vim: ts=2:sw=2:sts=2:expandtab
