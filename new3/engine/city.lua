local City = {}
City.__index = City

function City:new()
  local self = setmetatable({}, City)
  return self
end

function City:__tostring()
  return '[City]'
end

return City

-- vim: ts=2:sw=2:sts=2:expandtab
