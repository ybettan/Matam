#!/bin/tcsh -f

    
rm -f run4
rm -f *.o

gcc -std=c99 -c main.c parcer.c pokemon.c pokemonGo.c stock.c store.c trainer.c
gcc -std=c99 -pedantic-errors -Wall -Werror *.o -lm -L. -lmtm -o run4
if(!(-e "run4")) then
  echo "***********************************"
  echo "* Compilation error, test aborted *"
  echo "***********************************"
  exit
endif

rm -f out4
rm -f err4

(./run4 -p o_pokedex.in -e o_evolutions.in -l locations.in -i test4.in -o out4) >& err4

diff -w  out4 expout4
diff -w  err4 experr4


# checking memory leaks
(valgrind ./run4 -p o_pokedex.in -e o_evolutions.in -l locations.in -i test4.in -o /dev/null) >& valgrind_output4
set memory_leaks_number = \
  `grep "definitely lost:" valgrind_output4 | cut -f2 -d: | cut -f5 -d" "`
if($memory_leaks_number != "") echo $memory_leaks_number found

set memory_errors_number = \
  `grep "ERROR SUMMARY:" valgrind_output4 | cut -f2 -d: | cut -f2 -d" "`
if($memory_errors_number != 0) echo $memory_errors_number found

