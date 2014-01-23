local City = {}
City.__index = City

function City:new(x, y, tp, biome)
   local self = setmetatable({}, City)
   self.x, self.y = x, y
   local layout_number = math.random(1,#CityLayout[tp])
   local layout = CityLayout[tp][layout_number]
   self.w, self.h = layout.w, layout.h
   self.buildings = {}
   self.biome = biome
   self:__load_layout(layout)
   return self
end

function City:tiles(x, y)
   for _, bd in ipairs(self.buildings) do
      if x >= bd.x and x < (bd.x+bd.w) and y >= bd.y and y < (bd.y+bd.h) then
         return bd:tiles(x-bd.x, y-bd.y, self.biome)
      end
   end
   return { self.biome }
end

-------------
-- PRIVATE --
-------------

function City:__tostring()
   return '[City]'
end

function City:__load_layout(layout)
   for _,bd in ipairs(layout.buildings) do
      local building = Building:new(bd.layout, bd.x, bd.y)
      if building.x+building.w > self.w or building.y+building.h > self.h then
         error('Building outside city.')
      end
      self.buildings[#self.buildings+1] = building
   end
end

return City

-- vim: ts=3:sw=3:sts=3:expandtab
