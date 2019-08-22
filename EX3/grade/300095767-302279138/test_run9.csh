#!/bin/tcsh -f

    
rm -f run9
rm -f *.o

gcc -std=c99 -c main.c parcer.c pokemon.c pokemonGo.c stock.c store.c trainer.c
gcc -std=c99 -pedantic-errors -Wall -Werror *.o -lm -L. -lmtm -o run9
if(!(-e "run9")) then
  echo "***********************************"
  echo "* Compilation error, test aborted *"
  echo "***********************************"
  exit
endif

rm -f out9
rm -f err9

(./run9 -p pokedex.in -e evolutions.in -l locations.in -i test3.in -o out9) >& err9

diff -w  out9 expout9
diff -w  err9 experr9


# checking memory leaks
(valgrind ./run9 -p pokedex.in -e evolutions.in -l locations.in -i test3.in -o /dev/null) >& valgrind_output9
set memory_leaks_number = \
  `grep "definitely lost:" valgrind_output9 | cut -f2 -d: | cut -f5 -d" "`
if($memory_leaks_number != "") echo $memory_leaks_number found

set memory_errors_number = \
  `grep "ERROR SUMMARY:" valgrind_output9 | cut -f2 -d: | cut -f2 -d" "`
if($memory_errors_number != 0) echo $memory_errors_number found

