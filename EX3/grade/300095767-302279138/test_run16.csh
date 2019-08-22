#!/bin/tcsh -f

    
rm -f run16
rm -f *.o

gcc -std=c99 -pedantic-errors -Wall -Werror -DNDEBUG -c map_mtm/map_mtm.c map_mtm/map_test.c
gcc -std=c99 -pedantic-errors -Wall -Werror *.o -lm -L. -lmtm -o run16
if(!(-e "run16")) then
  echo "***********************************"
  echo "* Compilation error, test aborted *"
  echo "***********************************"
  exit
endif

rm -f out16
rm -f err16

(./run16 4 >! out16) >& err16

diff -w  out16 expout16
diff -w  err16 experr16


# checking memory leaks
(valgrind ./run16 4 >! /dev/null) >& valgrind_output16
set memory_leaks_number = \
  `grep "definitely lost:" valgrind_output16 | cut -f2 -d: | cut -f5 -d" "`
if($memory_leaks_number != "") echo $memory_leaks_number found

set memory_errors_number = \
  `grep "ERROR SUMMARY:" valgrind_output16 | cut -f2 -d: | cut -f2 -d" "`
if($memory_errors_number != 0) echo $memory_errors_number found

