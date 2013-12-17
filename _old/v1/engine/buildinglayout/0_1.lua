return {
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
    { type = 'Shopkeeper', x = 1, y = 1 },
  }
}

-- vim: ts=2:sw=2:sts=2:expandtab
