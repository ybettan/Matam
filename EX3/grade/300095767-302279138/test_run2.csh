#!/bin/tcsh -f

    
rm -f run2
rm -f *.o

gcc -std=c99 -c main.c parcer.c pokemon.c pokemonGo.c stock.c store.c trainer.c
gcc -std=c99 -pedantic-errors -Wall -Werror *.o -lm -L. -lmtm -o run2
if(!(-e "run2")) then
  echo "***********************************"
  echo "* Compilation error, test aborted *"
  echo "***********************************"
  exit
endif

rm -f out2
rm -f err2

(./run2 -p o_pokedex.in -e o_evolutions.in -l locations.in -i test2.in -o out2) >& err2

diff -w  out2 expout2
diff -w  err2 experr2


# checking memory leaks
(valgrind ./run2 -p o_pokedex.in -e o_evolutions.in -l locations.in -i test2.in -o /dev/null) >& valgrind_output2
set memory_leaks_number = \
  `grep "definitely lost:" valgrind_output2 | cut -f2 -d: | cut -f5 -d" "`
if($memory_leaks_number != "") echo $memory_leaks_number found

set memory_errors_number = \
  `grep "ERROR SUMMARY:" valgrind_output2 | cut -f2 -d: | cut -f2 -d" "`
if($memory_errors_number != 0) echo $memory_errors_number found

