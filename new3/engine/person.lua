local Person = {}
Person.__index = Person
Person.__tostring = function() return 'Person' end


function Person:new(x, y)
  local self = setmetatable({ }, Person)
  self.x = x
  self.y = y
  self.image = 0
  self.direction = 0
  self.__speed = 0
  return self
end


function Person:__tostring()
  return '[Person x:' .. self.x .. ' y:' .. self.y .. ']'
end


function Person:turn_to(dir)
  self.direction = dir
end


function Person:change_speed(speed)
  self.__speed = speed
end


function Person:step()
  if self.__speed == 0 then return end
  -- discover future position
  local step_size = 0.08 * self.__speed
  local fx = self.x + step_size * math.cos(self.direction)
  local fy = self.y + step_size * math.sin(self.direction)
  -- TODO - if it can't move, try changing the direction
  -- move
  if self:__can_move(fx, fy) then
    self.x, self.y = fx, fy
  end
end


function Person:__can_move(fx, fy)
  local tiles = {
    [world:unique_tile_id(math.floor(fx-0.5), math.floor(fy-0.5))] = true,
    [world:unique_tile_id(math.floor(fx+0.5), math.floor(fy-0.5))] = true,
    [world:unique_tile_id(math.floor(fx-0.5), math.floor(fy+0.5))] = true,
    [world:unique_tile_id(math.floor(fx+0.5), math.floor(fy+0.5))] = true
  }

  for k,_ in pairs(tiles) do
    if not world:tile_walkable(world:revert_unique_tile(k)) then
      return false
    end
  end

  return true
end

return Person

-- vim: ts=2:sw=2:sts=2:expandtab
