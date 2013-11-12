local str = {}

function str.wrap(st, columns)
  local r = { {} }
  for s in st:gmatch("([^ ]+)") do
    local line = r[#r]
    if #table.concat(line, ' ') + #s > columns then
      line = {}
      r[#r+1] = line
    end
    if s:byte(#s) == 10 then 
      line[#line+1] = s:sub(1,#s-1)
      line = {}
      r[#r+1] = line
    else
      line[#line+1] = s
    end
  end
  return funct.map(r, function(s) return table.concat(s, ' ') end)
end

return str

-- vim: ts=2:sw=2:sts=2:expandtab
