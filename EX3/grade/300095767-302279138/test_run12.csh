#!/bin/tcsh -f

    
rm -f run12
rm -f *.o

gcc -std=c99 -c main.c parcer.c pokemon.c pokemonGo.c stock.c store.c trainer.c
gcc -std=c99 -pedantic-errors -Wall -Werror *.o -lm -L. -lmtm -o run12
if(!(-e "run12")) then
  echo "***********************************"
  echo "* Compilation error, test aborted *"
  echo "***********************************"
  exit
endif

rm -f out12
rm -f err12

(./run12 -p pokedex.in -e evolutions.in -l locations.in -i test6.in -o out12) >& err12

diff -w  out12 expout12
diff -w  err12 experr12


# checking memory leaks
(valgrind ./run12 -p pokedex.in -e evolutions.in -l locations.in -i test6.in -o /dev/null) >& valgrind_output12
set memory_leaks_number = \
  `grep "definitely lost:" valgrind_output12 | cut -f2 -d: | cut -f5 -d" "`
if($memory_leaks_number != "") echo $memory_leaks_number found

set memory_errors_number = \
  `grep "ERROR SUMMARY:" valgrind_output12 | cut -f2 -d: | cut -f2 -d" "`
if($memory_errors_number != 0) echo $memory_errors_number found

