#!/bin/tcsh -f

    
rm -f run5
rm -f *.o

gcc -std=c99 -c main.c parcer.c pokemon.c pokemonGo.c stock.c store.c trainer.c
gcc -std=c99 -pedantic-errors -Wall -Werror *.o -lm -L. -lmtm -o run5
if(!(-e "run5")) then
  echo "***********************************"
  echo "* Compilation error, test aborted *"
  echo "***********************************"
  exit
endif

rm -f out5
rm -f err5

(./run5 -p o_pokedex.in -e o_evolutions.in -l locations.in -i test5.in -o out5) >& err5

diff -w  out5 expout5
diff -w  err5 experr5


# checking memory leaks
(valgrind ./run5 -p o_pokedex.in -e o_evolutions.in -l locations.in -i test5.in -o /dev/null) >& valgrind_output5
set memory_leaks_number = \
  `grep "definitely lost:" valgrind_output5 | cut -f2 -d: | cut -f5 -d" "`
if($memory_leaks_number != "") echo $memory_leaks_number found

set memory_errors_number = \
  `grep "ERROR SUMMARY:" valgrind_output5 | cut -f2 -d: | cut -f2 -d" "`
if($memory_errors_number != 0) echo $memory_errors_number found

