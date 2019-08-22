#!/bin/tcsh -f

    
rm -f run3
rm -f *.o

gcc -std=c99 -c main.c parcer.c pokemon.c pokemonGo.c stock.c store.c trainer.c
gcc -std=c99 -pedantic-errors -Wall -Werror *.o -lm -L. -lmtm -o run3
if(!(-e "run3")) then
  echo "***********************************"
  echo "* Compilation error, test aborted *"
  echo "***********************************"
  exit
endif

rm -f out3
rm -f err3

(./run3 -p o_pokedex.in -e o_evolutions.in -l locations.in -i test3.in -o out3) >& err3

diff -w  out3 expout3
diff -w  err3 experr3


# checking memory leaks
(valgrind ./run3 -p o_pokedex.in -e o_evolutions.in -l locations.in -i test3.in -o /dev/null) >& valgrind_output3
set memory_leaks_number = \
  `grep "definitely lost:" valgrind_output3 | cut -f2 -d: | cut -f5 -d" "`
if($memory_leaks_number != "") echo $memory_leaks_number found

set memory_errors_number = \
  `grep "ERROR SUMMARY:" valgrind_output3 | cut -f2 -d: | cut -f2 -d" "`
if($memory_errors_number != 0) echo $memory_errors_number found

