#!/bin/tcsh -f

    
rm -f run19
rm -f *.o

gcc -std=c99 -pedantic-errors -Wall -Werror -DNDEBUG -c map_mtm/map_mtm.c map_mtm/map_test.c
gcc -std=c99 -pedantic-errors -Wall -Werror *.o -lm -L. -lmtm -o run19
if(!(-e "run19")) then
  echo "***********************************"
  echo "* Compilation error, test aborted *"
  echo "***********************************"
  exit
endif

rm -f out19
rm -f err19

(./run19 7 >! out19) >& err19

diff -w  out19 expout19
diff -w  err19 experr19


# checking memory leaks
(valgrind ./run19 7 >! /dev/null) >& valgrind_output19
set memory_leaks_number = \
  `grep "definitely lost:" valgrind_output19 | cut -f2 -d: | cut -f5 -d" "`
if($memory_leaks_number != "") echo $memory_leaks_number found

set memory_errors_number = \
  `grep "ERROR SUMMARY:" valgrind_output19 | cut -f2 -d: | cut -f2 -d" "`
if($memory_errors_number != 0) echo $memory_errors_number found

