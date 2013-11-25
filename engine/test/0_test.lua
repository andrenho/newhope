--
-- Chunk 0: unit testing
--

function test_TestOK()
end

function test_Error()
  assert_error(function() return "a" + 1 end)
end

-- vim: ts=2:sw=2:sts=2:expandtab
