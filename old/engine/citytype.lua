local CityType = {
   AGRICULTURAL   = 10,
   REFINERY       = 20,
   MINING         = 30,
   FORESTAL       = 40,
   CHEMICAL       = 50,
   CAPITAL        = 60,
   FRONTIER       = 70,
   RANDOM         = 999,
}

CityType.random = function()
   local r = math.random(1,6)
   if r == 1 then
      return CityType.AGRICULTURAL
   elseif r == 2 then
      return CityType.REFINERY
   elseif r == 3 then
      return CityType.MINING
   elseif r == 4 then
      return CityType.CHEMICAL
   elseif r == 5 then
      return CityType.CAPITAL
   elseif r == 6 then
      return CityType.FRONTIER
   else
      assert(false)
   end
end

return CityType

-- vim: ts=3:sw=3:sts=3:expandtab
