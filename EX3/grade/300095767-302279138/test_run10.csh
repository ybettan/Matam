#!/bin/tcsh -f

    
rm -f run10
rm -f *.o

gcc -std=c99 -c main.c parcer.c pokemon.c pokemonGo.c stock.c store.c trainer.c
gcc -std=c99 -pedantic-errors -Wall -Werror *.o -lm -L. -lmtm -o run10
if(!(-e "run10")) then
  echo "***********************************"
  echo "* Compilation error, test aborted *"
  echo "***********************************"
  exit
endif

rm -f out10
rm -f err10

(./run10 -p pokedex.in -e evolutions.in -l locations.in -i test4.in -o out10) >& err10

diff -w  out10 expout10
diff -w  err10 experr10


# checking memory leaks
(valgrind ./run10 -p pokedex.in -e evolutions.in -l locations.in -i test4.in -o /dev/null) >& valgrind_output10
set memory_leaks_number = \
  `grep "definitely lost:" valgrind_output10 | cut -f2 -d: | cut -f5 -d" "`
if($memory_leaks_number != "") echo $memory_leaks_number found

set memory_errors_number = \
  `grep "ERROR SUMMARY:" valgrind_output10 | cut -f2 -d: | cut -f2 -d" "`
if($memory_errors_number != 0) echo $memory_errors_number found

