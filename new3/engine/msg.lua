local msg = {}

msg.MSGBOX = 1
msg.INPUT  = 2

--
-- Show a message to the user. If options != nil, display a option set to
-- the user and returns the chosen option. If person is set, means the person
-- is speaking.
--
function msg.msgbox(message, options, image, person)
  local id = nil
  if person then id = person:id() end
  return msg.callback(message, msg.MSGBOX, options, image, id)
end

--
-- Ask for a textual response from the user.
--
function msg.input(message, image, person)
  local id = nil
  if person then id = person:id() end
  return msg.callback(message, msg.INPUT, nil, image, id)
end

return msg

-- vim: ts=2:sw=2:sts=2:expandtab
