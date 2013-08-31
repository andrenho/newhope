#!/bin/sh

valgrind --leak-check=full --show-reachable=yes --track-origins=yes --suppressions=newhope.supp ./newhope
