#!/bin/tcsh -f

    
rm -f run15
rm -f *.o

gcc -std=c99 -pedantic-errors -Wall -Werror -DNDEBUG -c map_mtm/map_mtm.c map_mtm/map_test.c
gcc -std=c99 -pedantic-errors -Wall -Werror *.o -lm -L. -lmtm -o run15
if(!(-e "run15")) then
  echo "***********************************"
  echo "* Compilation error, test aborted *"
  echo "***********************************"
  exit
endif

rm -f out15
rm -f err15

(./run15 3 >! out15) >& err15

diff -w  out15 expout15
diff -w  err15 experr15


# checking memory leaks
(valgrind ./run15 3 >! /dev/null) >& valgrind_output15
set memory_leaks_number = \
  `grep "definitely lost:" valgrind_output15 | cut -f2 -d: | cut -f5 -d" "`
if($memory_leaks_number != "") echo $memory_leaks_number found

set memory_errors_number = \
  `grep "ERROR SUMMARY:" valgrind_output15 | cut -f2 -d: | cut -f2 -d" "`
if($memory_errors_number != 0) echo $memory_errors_number found

