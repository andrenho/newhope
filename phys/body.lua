-- http://engineeringdotnet.blogspot.com.br/2010/04/simple-2d-car-physics-in-games.html

local Body = {}
Body.__index = Body

function Body:new()
  local self = setmetatable({}, Body)
  return self
end

function Body:type()
  return 'Body'
end

-------------
-- PRIVATE --
-------------

function Body:__tostring()
  return '[Body]'
end

return Body

-- vim: ts=2:sw=2:sts=2:expandtab
