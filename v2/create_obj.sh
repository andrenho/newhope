#!/bin/sh

if [ "$1" == "" ]; then
  echo "Usage: $0 Classname" 1>&2
  exit 1
fi

FILENAME="${1,,}.lua"

cat << EOF > $FILENAME
local $1 = {}
$1.__index = $1

function $1:new()
  local self = setmetatable({}, $1)
  return self
end

function $1:type()
  return '$1'
end

-------------
-- PRIVATE --
-------------

function $1:__tostring()
  return '[$1]'
end

return $1

-- vim: ts=2:sw=2:sts=2:expandtab
EOF

git add $FILENAME

# vim: ts=2:sw=2:sts=2:expandtab
