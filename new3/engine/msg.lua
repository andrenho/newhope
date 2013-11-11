local msg = {}

--
-- Show a message to the user. If options != nil, display a option set to
-- the user and returns the chosen option. If person is set, means the person
-- is speaking.
--
function msg.msgbox(message, options, person, image)
  print(message)
end

--
-- Ask for a textual response from the user.
--
function msg.input(message, person, image)
end

return msg

-- vim: ts=2:sw=2:sts=2:expandtab
