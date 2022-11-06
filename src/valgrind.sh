#!/usr/bin/zsh
if [[ "$1" == "log" ]]; then
  valgrind --leak-check=full --show-leak-kinds=all --log-file=memleak.log ./a.out;
else
  valgrind --leak-check=full --show-leak-kinds=all ./a.out;
fi
