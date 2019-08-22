#!/bin/tcsh -f

    
rm -f run18
rm -f *.o

gcc -std=c99 -pedantic-errors -Wall -Werror -DNDEBUG -c map_mtm/map_mtm.c map_mtm/map_test.c
gcc -std=c99 -pedantic-errors -Wall -Werror *.o -lm -L. -lmtm -o run18
if(!(-e "run18")) then
  echo "***********************************"
  echo "* Compilation error, test aborted *"
  echo "***********************************"
  exit
endif

rm -f out18
rm -f err18

(./run18 6 >! out18) >& err18

diff -w  out18 expout18
diff -w  err18 experr18


# checking memory leaks
(valgrind ./run18 6 >! /dev/null) >& valgrind_output18
set memory_leaks_number = \
  `grep "definitely lost:" valgrind_output18 | cut -f2 -d: | cut -f5 -d" "`
if($memory_leaks_number != "") echo $memory_leaks_number found

set memory_errors_number = \
  `grep "ERROR SUMMARY:" valgrind_output18 | cut -f2 -d: | cut -f2 -d" "`
if($memory_errors_number != 0) echo $memory_errors_number found

