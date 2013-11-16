local A = {}
A.__index = A

function A:new()
  local self = setmetatable({}, A)
  self.z = 4
  return self
end

function A:x()
  return 'A:x'
end

local B = A:new()
B.__index = B

function B:new()
  local self = setmetatable({}, B)
  return self
end

function B:x()
  return 'B' .. A:x()
end

function B:y()
  return 'B:y'
end

a = A:new()
b = B:new()
print('4', a.z)
print('4', b.z)
print('A:x', A:x())
print('BA:x', b:x())
print('B:y', b:y())
function B:x() return 'B:x' end
print('B:x', b:x())
function B:x() return self.z end
print('4', b:x())


-- vim: ts=2:sw=2:sts=2:expandtab
