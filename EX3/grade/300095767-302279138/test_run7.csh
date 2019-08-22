#!/bin/tcsh -f

    
rm -f run7
rm -f *.o

gcc -std=c99 -c main.c parcer.c pokemon.c pokemonGo.c stock.c store.c trainer.c
gcc -std=c99 -pedantic-errors -Wall -Werror *.o -lm -L. -lmtm -o run7
if(!(-e "run7")) then
  echo "***********************************"
  echo "* Compilation error, test aborted *"
  echo "***********************************"
  exit
endif

rm -f out7
rm -f err7

(./run7 -p pokedex.in -e evolutions.in -l locations.in -i test1.in -o out7) >& err7

diff -w  out7 expout7
diff -w  err7 experr7


# checking memory leaks
(valgrind ./run7 -p pokedex.in -e evolutions.in -l locations.in -i test1.in -o /dev/null) >& valgrind_output7
set memory_leaks_number = \
  `grep "definitely lost:" valgrind_output7 | cut -f2 -d: | cut -f5 -d" "`
if($memory_leaks_number != "") echo $memory_leaks_number found

set memory_errors_number = \
  `grep "ERROR SUMMARY:" valgrind_output7 | cut -f2 -d: | cut -f2 -d" "`
if($memory_errors_number != 0) echo $memory_errors_number found

