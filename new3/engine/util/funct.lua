function filter(table, func)
  local new_table = {}
  for _,v in ipairs(table) do
    if func(v) then new_table[#new_table+1] = v end
  end
  return new_table
end

return {
  filter=filter,
}

-- vim: ts=2:sw=2:sts=2:expandtab
