function objhash()
   return setmetatable({ _proxy = {} }, {
      __newindex = function(t, k, v)
         print(k)
         t._proxy[k:__hash()] = v
      end,
      __index = function(t, k)
         return t._proxy[k:__hash()]
      end
   })
end

-- vim: ts=3:sw=3:sts=3:expandtab
