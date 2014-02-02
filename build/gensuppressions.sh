#!/bin/sh

valgrind --leak-check=full --show-reachable=yes --suppressions=build/staticinit.txt --gen-suppressions=all ./newhope 2>&1 >/dev/null | grep -v "=.*" > build/suppressions.txt
