#!/bin/tcsh -f

    
rm -f run14
rm -f *.o

gcc -std=c99 -pedantic-errors -Wall -Werror -DNDEBUG -c map_mtm/map_mtm.c map_mtm/map_test.c
gcc -std=c99 -pedantic-errors -Wall -Werror *.o -lm -L. -lmtm -o run14
if(!(-e "run14")) then
  echo "***********************************"
  echo "* Compilation error, test aborted *"
  echo "***********************************"
  exit
endif

rm -f out14
rm -f err14

(./run14 2 >! out14) >& err14

diff -w  out14 expout14
diff -w  err14 experr14


# checking memory leaks
(valgrind ./run14 2 >! /dev/null) >& valgrind_output14
set memory_leaks_number = \
  `grep "definitely lost:" valgrind_output14 | cut -f2 -d: | cut -f5 -d" "`
if($memory_leaks_number != "") echo $memory_leaks_number found

set memory_errors_number = \
  `grep "ERROR SUMMARY:" valgrind_output14 | cut -f2 -d: | cut -f2 -d" "`
if($memory_errors_number != 0) echo $memory_errors_number found

