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
  -- general
  'step',
  'add_static_tile',
  -- person
  'create_dynamic_person_body',
  'create_static_person_body',
  'set_dynamic_person_target',
  -- player
  'setup_player_collision_handler',
}

function Game:new(w, cb)
  assert(w:type() == 'World')
  local self = setmetatable({}, Game)
  self.__callbacks = self:__check_callbacks(cb)

  -- setup globals
  self:__setup_physics_callbacks(cb)
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
    local collisions = self.__physics_step(world.objects)
    world:step(collisions)
    -- draw screen
    local x1,y1,x2,y2 = self.__callbacks.window_tiles()
    local objects = world:objects_in_area(x1,y1,x2,y2)
    self.__callbacks.render(objects, x1, y1, x2, y2)
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

function Game:__execute_commands(cmd)
  x, y = 0, 0
  if cmd.up then y = -1 elseif cmd.down then y = 1 end
  if cmd.left then x = -1 elseif cmd.right then x = 1 end
  world.player:set_target(world.player.x + (x*1000000), world.player.y + (y*1000000))
end


function Game:__check_callbacks(cb)
  local missing = {}
  for _,cb_name in ipairs(Game.__required_callbacks) do
    if not cb[cb_name] then missing[#missing+1] = cb_name end
  end
  if #missing > 0 then error('Callbacks missing: '..table.concat(missing, ', '), 2) end
  return cb
end


function Game:__setup_physics_callbacks(cb)
  local missing = {}
  for _,cb_name in ipairs(Game.__physics_callbacks) do
    if not cb[cb_name] then missing[#missing+1] = cb_name end
  end
  if #missing > 0 then error('Physics callbacks missing: '..table.concat(missing, ', '), 2) end
  self.__physics_step = self.__callbacks.step
  World.__physics_add_solid_tile = self.__callbacks.add_static_tile
  StaticPerson.create_physics_body = self.__callbacks.create_static_person_body
  DynamicPerson.create_physics_body = self.__callbacks.create_dynamic_person_body
  DynamicPerson.set_target = self.__callbacks.set_dynamic_person_target
  Player.setup_collision_handler = self.__callbacks.setup_player_collision_handler
end


function Game:__physics_step(collisions)
  error('This function should have been replaced by a C function.')
end


function Game:__tostring()
  return '[Game]'
end

return Game

-- vim: ts=2:sw=2:sts=2:expandtab
