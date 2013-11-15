local City = {}
City.__index = City

function City:new(layout, x, y)
  local self = setmetatable({}, City)
  self.x, self.y = x, y
  self.__buildings = {}
  self:__load_layout(layout)
  return self
end

function City:__tostring()
  return '[City]'
end

function City:__load_layout(layout)
  mod = string.format('citylayout.layout_%d', layout)
  package.loaded[mod] = nil
  layout = require(mod)
  for _,bd in ipairs(layout.buildings) do
    self.__buildings[#self.__buildings+1] = Building:new(bd.btype, bd.layout, bd.x, bd.y)
  end
end

return City

-- vim: ts=2:sw=2:sts=2:expandtab
