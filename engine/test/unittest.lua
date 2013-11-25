-- assert functions
function assert_error(expr)
  assert(not pcall(expr), "expression should have raised error")
end

-- load classes
package.path = ";;../?.lua;./?.lua"
require('newhope')

-- load tests
require('setup')
for filename in io.popen('ls *.lua'):lines() do
  if filename ~= 'unittest.lua' then
    require(filename:sub(1, #filename-4))
  end
end

-- run tests
local errors, count = {}, 0
for name,fct in pairs(_G) do
  if name:sub(1, 5) == 'test_' then
    count = count + 1
    setup()
    local ok, msg = pcall(fct)
    if ok then
      io.write('.')
    else
      errors[#errors+1] = '[' .. name .. '] ' .. msg
      if msg:find('assertion failed') then io.write('F') else io.write('E') end
    end
    teardown()
  end
end

-- print report
print('')
for _,err in ipairs(errors) do
  print(err)
end
print(('Tests finshed: %d successes, %d failures.'):format(count-#errors, #errors))

-- vim: ts=2:sw=2:sts=2:expandtab
