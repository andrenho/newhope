local TalkQueue = {}
TalkQueue.__index = TalkQueue

function TalkQueue:new()
  local self = setmetatable({}, TalkQueue)
  self.__first = nil
  self.__last = nil
  return self
end


function TalkQueue:enqueue(from, to, message, parameters)
  print(tostring(from), tostring(to), message)
  local pack = { from=from, to=to, message=message, parameters=parameters, prev=nil, next=nil }
  if self.__first then -- not empty
    pack.prev = self.__last
    self.__last.next = pack
  else
    self.__first = pack
  end
  self.__last = pack
end


function TalkQueue:peek_from(from_filter)
  local q = self.__first
  while q do
    if q.from == from_filter then return q end
    q = q.next
  end
  return nil
end


function TalkQueue:peek_to(to_filter)
  local q = self.__first
  while q do
    if q.to == to_filter then return q end
    q = q.next
  end
  return nil
end


function TalkQueue:dequeue(to_filter)
  local q = self.__first
  while q do
    if q.to == to_filter then
      local pack = q
      if q.prev then q.prev.next = q.next else self.__first = q.next end
      if q.next then q.next.prev = q.prev else self.__last = q.prev end
      return q
    end
    q = q.next
  end
  return nil
end

-------------
-- PRIVATE --
-------------

function TalkQueue:__tostring()
  return '[TalkQueue]'
end

return TalkQueue

-- vim: ts=2:sw=2:sts=2:expandtab
