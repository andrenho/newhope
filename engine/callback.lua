local Callback = {}
Callback.__index = Callback

function Callback:new()
  local self = setmetatable({}, Callback)
  self.called = false
  return self
end

function Callback:missing()
  return "" -- TODO
end

function Callback:all_installed()
  return false -- TODO
end

function Callback:install(name, fct)
  -- TODO
end

function Callback:run()
  -- TODO
end

-------------
-- PRIVATE --
-------------

function Callback:__tostring()
  return '[Callback]'
end

return Callback

-- vim: ts=2:sw=2:sts=2:expandtab
