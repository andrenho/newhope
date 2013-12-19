local mod = {
  loaded = {}
}

function mod.require(modname)
  if mod.loaded[modname] then
    package.loaded[modname] = nil
  end
  mod.loaded[modname] = true
  return require(modname)
end

return mod

-- vim: ts=2:sw=2:sts=2:expandtab
