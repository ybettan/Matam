#!/bin/tcsh -f

    
rm -f run13
rm -f *.o

gcc -std=c99 -pedantic-errors -Wall -Werror -DNDEBUG -c map_mtm/map_mtm.c map_mtm/map_test.c
gcc -std=c99 -pedantic-errors -Wall -Werror *.o -lm -L. -lmtm -o run13
if(!(-e "run13")) then
  echo "***********************************"
  echo "* Compilation error, test aborted *"
  echo "***********************************"
  exit
endif

rm -f out13
rm -f err13

(./run13 1 >! out13) >& err13

diff -w  out13 expout13
diff -w  err13 experr13


# checking memory leaks
(valgrind ./run13 1 >! /dev/null) >& valgrind_output13
set memory_leaks_number = \
  `grep "definitely lost:" valgrind_output13 | cut -f2 -d: | cut -f5 -d" "`
if($memory_leaks_number != "") echo $memory_leaks_number found

set memory_errors_number = \
  `grep "ERROR SUMMARY:" valgrind_output13 | cut -f2 -d: | cut -f2 -d" "`
if($memory_errors_number != 0) echo $memory_errors_number found

