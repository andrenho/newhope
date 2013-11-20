local TalkQueue = {}
TalkQueue.__index = TalkQueue

function TalkQueue:new()
  local self = setmetatable({}, TalkQueue)
  self.__queue = nil
  self.__last = nil
  return self
end

function TalkQueue:enqueue(from, to, message, parameters)
  local pack = { from=from, to=to, message=message, parameters=parameters, prev=nil, next=nil }
  if self.__queue then
    pack.__prev = self.__last
    self.__last.next = pack
  else
    self.__last = pack
    self.__queue = pack
  end
end

function TalkQueue:dequeue(to_filter)
  local q = self.__queue
  while q do
    print(q.to)
    if q.to == to_filter then
      local pack = q
      
      return q
    end
    q = q.next
  end
end

-------------
-- PRIVATE --
-------------

function TalkQueue:__tostring()
  return '[TalkQueue]'
end

--return TalkQueue

t = TalkQueue:new()
t:enqueue('a', 'b', 'message 1', {})
t:enqueue('b', 'a', 'message 2', {})
t:enqueue('a', 'b', 'message 3', {})
t:enqueue('a', 'b', 'message 4', {})
--print(t:dequeue('b').message)
--print(t:dequeue('b').message)
print(t:dequeue('a').message)

-- vim: ts=2:sw=2:sts=2:expandtab
