local BuildingLayout = {
   GENERAL_STORE = {
      [1] = { '        ',
              ' ffffff ',
              ' ffffff ',
              ' ffffff ',
              '        ', },
      [2] = { 'wwwwwwww',
              'w      w',
              'w      w',
              'w      w',
              'wwwwddww', },
      [3] = { 'wwwwwwww',
              'w      w',
              'w      w',
              'w      w',
              'wwwwddww', },
      [4] = { 'wwwwwwww',
              'wwwwwwww',
              'wwwwwwww',
              'wwwwwwww',
              'wwwwwwww', },
        
      [' '] = Block.NOTHING,
      ['f'] = Block.FLOOR,
      ['w'] = Block.WOODEN_WALL,
      ['d'] = Block.DOOR_OPEN,

      people = {
         { type = 'Shopkeeper', x = 0.5, y = 0.5 },
      }
   },
}

BuildingLayout.CAR_DEALERSHIP = table.deepcopy(BuildingLayout.GENERAL_STORE)
BuildingLayout.CAR_DEALERSHIP.people[1].type = 'CarDealer'

BuildingLayout.BAR = table.deepcopy(BuildingLayout.GENERAL_STORE)
BuildingLayout.BAR.people[1].type = 'Bartender'

BuildingLayout.MERCHANT_UNION = table.deepcopy(BuildingLayout.GENERAL_STORE)
BuildingLayout.MERCHANT_UNION.people[1].type = 'Secretary'

BuildingLayout.GUNS_FOR_HIRE = table.deepcopy(BuildingLayout.GENERAL_STORE)
BuildingLayout.GUNS_FOR_HIRE.people[1].type = 'Dispatcher'

BuildingLayout.INFIRMARY = table.deepcopy(BuildingLayout.GENERAL_STORE)
BuildingLayout.INFIRMARY.people[1].type = 'Medic'


return BuildingLayout

-- vim: ts=3:sw=3:sts=3:expandtab
