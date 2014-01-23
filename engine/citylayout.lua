local CityLayout = {
   [CityType.AGRICULTURAL] = {
      {
         w = 40,
         h = 20,
         buildings = {
            { layout=BuildingLayout.GENERAL_STORE, x=0, y=0 },
            { layout=BuildingLayout.CAR_DEALERSHIP, x=10, y=0 },
            { layout=BuildingLayout.BAR, x=20, y=2 },
         },
      },
   },
}

CityLayout[CityType.REFINERY] = CityLayout[CityType.AGRICULTURAL]
CityLayout[CityType.MINING]   = CityLayout[CityType.AGRICULTURAL]
CityLayout[CityType.FORESTAL] = CityLayout[CityType.AGRICULTURAL]
CityLayout[CityType.CHEMICAL] = CityLayout[CityType.AGRICULTURAL]
CityLayout[CityType.CAPITAL]  = CityLayout[CityType.AGRICULTURAL]
CityLayout[CityType.FRONTIER] = CityLayout[CityType.AGRICULTURAL]

return CityLayout

-- vim: ts=3:sw=3:sts=3:expandtab
