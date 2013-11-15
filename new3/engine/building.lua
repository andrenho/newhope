local Building = {}
Building.__index = Building

function Building:new(btype, layout, x, y)
  local self = setmetatable({}, Building)
  self.x, self.y = x, y
  -- TODO load layout
  return self
end

function Building:__tostring()
  return '[Building]'
end

return Building

-- vim: ts=2:sw=2:sts=2:expandtab
