#!/bin/tcsh -f

    
rm -f run17
rm -f *.o

gcc -std=c99 -pedantic-errors -Wall -Werror -DNDEBUG -c map_mtm/map_mtm.c map_mtm/map_test.c
gcc -std=c99 -pedantic-errors -Wall -Werror *.o -lm -L. -lmtm -o run17
if(!(-e "run17")) then
  echo "***********************************"
  echo "* Compilation error, test aborted *"
  echo "***********************************"
  exit
endif

rm -f out17
rm -f err17

(./run17 5 >! out17) >& err17

diff -w  out17 expout17
diff -w  err17 experr17


# checking memory leaks
(valgrind ./run17 5 >! /dev/null) >& valgrind_output17
set memory_leaks_number = \
  `grep "definitely lost:" valgrind_output17 | cut -f2 -d: | cut -f5 -d" "`
if($memory_leaks_number != "") echo $memory_leaks_number found

set memory_errors_number = \
  `grep "ERROR SUMMARY:" valgrind_output17 | cut -f2 -d: | cut -f2 -d" "`
if($memory_errors_number != 0) echo $memory_errors_number found

