function filter(table, func)
   local new_table = {}
   for _,v in ipairs(table) do
      if func(v) then new_table[#new_table+1] = v end
   end
   return new_table
end

function map(table, func)
   local new_table = {}
   for _,v in ipairs(table) do
      new_table[#new_table+1] = func(v)
   end
   return new_table
end

function min(tbl, func)
   local t = table.shallow_copy(tbl)
   if func then
      table.sort(t, func)
   else
      table.sort(t)
   end
   return t[1]
end

function max(tbl, func)
   local t = table.shallow_copy(tbl)
   if func then
      table.sort(t, func)
   else
      table.sort(t)
   end
   return t[#t]
end

function avg(tbl, func)
   local f = func or function(p) return p end
   local s = 0.0
   for _,v in ipairs(tbl) do s = s+f(v) end
   return s/#tbl
end

return {
   map=map,
   filter=filter,
   min=min,
   max=max,
   avg=avg,
}

-- vim: ts=3:sw=3:sts=3:expandtab
