#!/bin/tcsh -f

    
rm -f run11
rm -f *.o

gcc -std=c99 -c main.c parcer.c pokemon.c pokemonGo.c stock.c store.c trainer.c
gcc -std=c99 -pedantic-errors -Wall -Werror *.o -lm -L. -lmtm -o run11
if(!(-e "run11")) then
  echo "***********************************"
  echo "* Compilation error, test aborted *"
  echo "***********************************"
  exit
endif

rm -f out11
rm -f err11

(./run11 -p pokedex.in -e evolutions.in -l locations.in -i test5.in -o out11) >& err11

diff -w  out11 expout11
diff -w  err11 experr11


# checking memory leaks
(valgrind ./run11 -p pokedex.in -e evolutions.in -l locations.in -i test5.in -o /dev/null) >& valgrind_output11
set memory_leaks_number = \
  `grep "definitely lost:" valgrind_output11 | cut -f2 -d: | cut -f5 -d" "`
if($memory_leaks_number != "") echo $memory_leaks_number found

set memory_errors_number = \
  `grep "ERROR SUMMARY:" valgrind_output11 | cut -f2 -d: | cut -f2 -d" "`
if($memory_errors_number != 0) echo $memory_errors_number found

