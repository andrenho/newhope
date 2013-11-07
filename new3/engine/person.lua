local Person = {}
Person.__index = Person

function Person:new()
  local self = setmetatable({}, Person)
  return self
end

return Person

-- vim: ts=2:sw=2:sts=2:expandtab
