function objhash()
   return setmetatable({ data = {} }, {
      __newindex = function(t, k, v)
         t.data[k:__hash()] = v
      end,
      __index = function(t, k)
         return t.data[k:__hash()]
      end
   })
end

return objhash

-- vim: ts=3:sw=3:sts=3:expandtab
