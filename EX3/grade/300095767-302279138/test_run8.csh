#!/bin/tcsh -f

    
rm -f run8
rm -f *.o

gcc -std=c99 -c main.c parcer.c pokemon.c pokemonGo.c stock.c store.c trainer.c
gcc -std=c99 -pedantic-errors -Wall -Werror *.o -lm -L. -lmtm -o run8
if(!(-e "run8")) then
  echo "***********************************"
  echo "* Compilation error, test aborted *"
  echo "***********************************"
  exit
endif

rm -f out8
rm -f err8

(./run8 -p pokedex.in -e evolutions.in -l locations.in -i test2.in -o out8) >& err8

diff -w  out8 expout8
diff -w  err8 experr8


# checking memory leaks
(valgrind ./run8 -p pokedex.in -e evolutions.in -l locations.in -i test2.in -o /dev/null) >& valgrind_output8
set memory_leaks_number = \
  `grep "definitely lost:" valgrind_output8 | cut -f2 -d: | cut -f5 -d" "`
if($memory_leaks_number != "") echo $memory_leaks_number found

set memory_errors_number = \
  `grep "ERROR SUMMARY:" valgrind_output8 | cut -f2 -d: | cut -f2 -d" "`
if($memory_errors_number != 0) echo $memory_errors_number found

