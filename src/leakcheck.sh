#!/bin/bash
#--gen-suppressions=all
valgrind --leak-check=yes --track-origins=yes ./main 2>&1 | tee valgrind.log
