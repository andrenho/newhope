local Building = {}
Building.__index = Building

function Building:new()
  local self = setmetatable({}, Building)
  return self
end

function Building:__tostring()
  return '[Building]'
end

return Building

-- vim: ts=2:sw=2:sts=2:expandtab
