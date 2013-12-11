local Game = {}
Game.__index = Game

world = nil -- global variable world

Game.__required_callbacks = {
}

function Game:new(w, callbacks)
  world = w
  local self = setmetatable({}, Game)
  self.callbacks = self:__check_callbacks(callbacks)
  return self
end

function Game:start()
  print 'started'
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
  if #missing > 0 then error('Callbacks missing: '..table.concat(missing, ' ')) end
  return cb
end

function Game:__tostring()
  return '[Game]'
end

return Game

-- vim: ts=2:sw=2:sts=2:expandtab
