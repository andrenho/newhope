local CityLayout = {
   [CityType.AGRICULTURAL] = {
      {
         w = 70,
         h = 20,
         buildings = {
            { layout=BuildingLayout.GENERAL_STORE, x=0, y=0 },
            { layout=BuildingLayout.CAR_DEALERSHIP, x=10, y=0 },
            { layout=BuildingLayout.BAR, x=20, y=0 },
            { layout=BuildingLayout.MERCHANT_UNION, x=30, y=0 },
            { layout=BuildingLayout.GUNS_FOR_HIRE, x=40, y=0 },
            { layout=BuildingLayout.INFIRMARY, x=50, y=0 },
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
