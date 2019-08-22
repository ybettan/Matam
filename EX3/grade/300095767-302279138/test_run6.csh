#!/bin/tcsh -f

    
rm -f run6
rm -f *.o

gcc -std=c99 -c main.c parcer.c pokemon.c pokemonGo.c stock.c store.c trainer.c
gcc -std=c99 -pedantic-errors -Wall -Werror *.o -lm -L. -lmtm -o run6
if(!(-e "run6")) then
  echo "***********************************"
  echo "* Compilation error, test aborted *"
  echo "***********************************"
  exit
endif

rm -f out6
rm -f err6

(./run6 -p o_pokedex.in -e o_evolutions.in -l locations.in -i test6.in -o out6) >& err6

diff -w  out6 expout6
diff -w  err6 experr6


# checking memory leaks
(valgrind ./run6 -p o_pokedex.in -e o_evolutions.in -l locations.in -i test6.in -o /dev/null) >& valgrind_output6
set memory_leaks_number = \
  `grep "definitely lost:" valgrind_output6 | cut -f2 -d: | cut -f5 -d" "`
if($memory_leaks_number != "") echo $memory_leaks_number found

set memory_errors_number = \
  `grep "ERROR SUMMARY:" valgrind_output6 | cut -f2 -d: | cut -f2 -d" "`
if($memory_errors_number != 0) echo $memory_errors_number found

