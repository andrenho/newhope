local Building = {}
Building.__index = Building

function Building:new(btype, layout, x, y)
  local self = setmetatable({}, Building)
  self.x, self.y = x, y
  self:__load_layout(btype, layout)
  self.w = #self.layout[1][1]
  self.h = #self.layout[1]
  return self
end

function Building:tiles(x, y, basic_block)
  local i = 1
  local stack = {}
  while self.layout[i] do
    local block = self.layout[self.layout[i][y+1]:sub(x+1,x+1)]
    if block == Block.NOTHING and i == 1 and basic_block then block = basic_block end
    stack[#stack+1] = block
    i = i+1
  end
  return stack
end

-------------
-- PRIVATE --
-------------

function Building:__tostring()
  return '[Building]'
end

function Building:__load_layout(btype, layout)
  mod = string.format('buildinglayout.%d_%d', btype, layout)
  package.loaded[mod] = nil
  self.layout = require(mod)
end

return Building

-- vim: ts=2:sw=2:sts=2:expandtab
