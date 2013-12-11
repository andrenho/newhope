package.path = ';;?.lua;./geo/?.lua;../geo/?.lua'

return {
  Point = my_require('point'),
  Segment = my_require('segment'),
  Shape = my_require('shape'),
  Rectangle = my_require('rectangle'),
  Vector = my_require('vector'),
}

-- vim: ts=2:sw=2:sts=2:expandtab
