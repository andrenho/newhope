#!/bin/sh

valgrind --leak-check=full --show-reachable=yes --track-origins=yes --suppressions=build/suppressions.txt ./newhope
