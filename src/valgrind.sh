#!/usr/bin/zsh
valgrind --leak-check=full --show-leak-kinds=all --log-file=memleak.log ./a.out;
