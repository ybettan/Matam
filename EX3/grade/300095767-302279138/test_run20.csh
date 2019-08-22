#!/bin/tcsh -f

    
rm -f run20
rm -f *.o

gcc -std=c99 -pedantic-errors -Wall -Werror -DNDEBUG -c map_mtm/map_mtm.c map_mtm/map_test.c
gcc -std=c99 -pedantic-errors -Wall -Werror *.o -lm -L. -lmtm -o run20
if(!(-e "run20")) then
  echo "***********************************"
  echo "* Compilation error, test aborted *"
  echo "***********************************"
  exit
endif

rm -f out20
rm -f err20

(./run20 8 >! out20) >& err20

diff -w  out20 expout20
diff -w  err20 experr20


# checking memory leaks
(valgrind ./run20 8 >! /dev/null) >& valgrind_output20
set memory_leaks_number = \
  `grep "definitely lost:" valgrind_output20 | cut -f2 -d: | cut -f5 -d" "`
if($memory_leaks_number != "") echo $memory_leaks_number found

set memory_errors_number = \
  `grep "ERROR SUMMARY:" valgrind_output20 | cut -f2 -d: | cut -f2 -d" "`
if($memory_errors_number != 0) echo $memory_errors_number found

