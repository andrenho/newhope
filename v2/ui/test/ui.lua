function initialize_ui()
end

function active()
  return true
end

function current_time_us()
  return 0
end

function sleep_us()
end

function get_commands()
  return {}
end

function render(units)
end

function window_tiles()
  return { 0, 0, 0, 0 }
end

return {
  initialize_ui   = initialize_ui,
  active          = active,
  current_time_us = current_time_us,
  sleep_us        = sleep_us,
  get_commands    = get_commands,
  render          = render,
  window_tiles    = window_tiles,
}

-- vim: ts=2:sw=2:sts=2:expandtab
