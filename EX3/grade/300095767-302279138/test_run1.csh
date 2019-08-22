#!/bin/tcsh -f

    
rm -f run1
rm -f *.o

gcc -std=c99 -c main.c parcer.c pokemon.c pokemonGo.c stock.c store.c trainer.c
gcc -std=c99 -pedantic-errors -Wall -Werror *.o -lm -L. -lmtm -o run1
if(!(-e "run1")) then
  echo "***********************************"
  echo "* Compilation error, test aborted *"
  echo "***********************************"
  exit
endif

rm -f out1
rm -f err1

(./run1 -p o_pokedex.in -e o_evolutions.in -l locations.in -i test1.in -o out1) >& err1

diff -w  out1 expout1
diff -w  err1 experr1


# checking memory leaks
(valgrind ./run1 -p o_pokedex.in -e o_evolutions.in -l locations.in -i test1.in -o /dev/null) >& valgrind_output1
set memory_leaks_number = \
  `grep "definitely lost:" valgrind_output1 | cut -f2 -d: | cut -f5 -d" "`
if($memory_leaks_number != "") echo $memory_leaks_number found

set memory_errors_number = \
  `grep "ERROR SUMMARY:" valgrind_output1 | cut -f2 -d: | cut -f2 -d" "`
if($memory_errors_number != 0) echo $memory_errors_number found

