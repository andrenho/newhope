local Callback = {}
Callback.__index = Callback

Callback.missing = { 
  message = true,
}

function Callback:new()
  local self = setmetatable({}, Callback)
  return self
end


function Callback:install(name, fct)
  self[name] = fct
  self.missing[name] = nil
end

-------------
-- PRIVATE --
-------------

function Callback:__tostring()
  return '[Callback]'
end

return Callback

-- vim: ts=2:sw=2:sts=2:expandtab
