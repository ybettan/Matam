#include <string.h>

#include "test_utils.h"

#ifdef P
// Pokemon tests.                             10
bool TestInvalidArgs();               // 1  | 1
bool TestDefaultTypes();              // 2  | 1
bool TestCopyAndAssignment();         // 3  | 2
bool TestComparison();                // 4  | 2
bool TestLevel();                     // 5  | 1
bool TestHit();                       // 6  | 1
bool TestHeal();                      // 7  | 1
bool TestTrain();                     // 8  | 1
#endif

#ifdef T
// Trainer tests.                             10
bool TestGetStrongestPokemon();       // 9  | 2
bool TestKillStrongestPokemon();      // 10 | 2
bool TestIsAlly();                    // 11 | 2
bool TestGetTeam();                   // 12 | 2
bool TestTryToCatch();                // 13 | 2
#endif

#ifdef PG
// PokemonGo tests.                           50
bool TestAddTrainerExceptions();      // 14 | 2
bool TestWhereIsExceptions();         // 15 | 2
bool TestGetTrainersInExceptions();   // 16 | 2
bool TestMoveTrainerExceptions();     // 17 | 2
bool TestPokemonGo0();                // 18 | 6
bool TestPokemonGo1();                // 19 | 6
bool TestPokemonGo2();                // 20 | 6
bool TestPokemonGo3();                // 21 | 6
#endif

#ifdef G
// KGraph tests.                              30
bool TestOperatorSubscript();         // 25 | 3
bool TestContains();                  // 26 | 3
bool TestInsert();                    // 27 | 3
bool TestRemove();                    // 28 | 3
bool TestIterators();                 // 29 | 4
bool TestConstIterators();            // 30 | 2
bool TestConnect();                   // 31 | 4
bool TestDisconnect();                // 32 | 4
bool TestCopy();                      // 33 | 4
#endif

int main(int argc, char* argv[]) {
  int test = atoi(argv[1]);
  switch(test) {
    case 1: {
      #ifdef P
      RUN_TEST(TestInvalidArgs);
      #endif
      break;
    }
    case 2: {
      #ifdef P
      RUN_TEST(TestDefaultTypes);
      #endif
      break;
    }
    case 3: {
      #ifdef P
      RUN_TEST(TestCopyAndAssignment);
      #endif
      break;
    }
    case 4: {
      #ifdef P
      RUN_TEST(TestComparison);
      #endif
      break;
    }
    case 5: {
      #ifdef P
      RUN_TEST(TestLevel);
      #endif
      break;
    }
    case 6: {
      #ifdef P
      RUN_TEST(TestHit);
      #endif
      break;
    }
    case 7: {
      #ifdef P
      RUN_TEST(TestHeal);
      #endif
      break;
    }
    case 8: {
      #ifdef P
      RUN_TEST(TestTrain);
      #endif
      break;
    }
    case 9: {
      #ifdef T
      RUN_TEST(TestGetStrongestPokemon);
      #endif
      break;
    }
    case 10: {
      #ifdef T
      RUN_TEST(TestKillStrongestPokemon);
      #endif
      break;
    }
    case 11: {
      #ifdef T
      RUN_TEST(TestIsAlly);
      #endif
      break;
    }
    case 12: {
      #ifdef T
      RUN_TEST(TestGetTeam);
      #endif
      break;
    }
    case 13: {
      #ifdef T
      RUN_TEST(TestTryToCatch);
      #endif
      break;
    }
    case 14: {
      #ifdef PG
      RUN_TEST(TestAddTrainerExceptions);
      #endif
      break;
    }
    case 15: {
      #ifdef PG
      RUN_TEST(TestWhereIsExceptions);
      #endif
      break;
    }
    case 16: {
      #ifdef PG
      RUN_TEST(TestGetTrainersInExceptions);
      #endif
      break;
    }
    case 17: {
      #ifdef PG
      RUN_TEST(TestMoveTrainerExceptions);
      #endif
      break;
    }
    case 18: {
      #ifdef PG
      RUN_TEST(TestPokemonGo0);
      #endif
      break;
    }
    case 19: {
      #ifdef PG
      RUN_TEST(TestPokemonGo1);
      #endif
      break;
    }
    case 20: {
      #ifdef PG
      RUN_TEST(TestPokemonGo2);
      #endif
      break;
    }
    case 21: {
      #ifdef PG
      RUN_TEST(TestPokemonGo3);
      #endif
      break;
    }
    case 22: {
      #ifdef PG
      
      #endif
      break;
    }
    case 23: {
      #ifdef PG
      
      #endif
      break;
    }
    case 24: {
      #ifdef PG
      
      #endif
      break;
    }
    case 25: {
      #ifdef G
      RUN_TEST(TestOperatorSubscript);
      #endif
      break;
    }
    case 26: {
      #ifdef G
      RUN_TEST(TestContains);
      #endif
      break;
    }
    case 27: {
      #ifdef G
      RUN_TEST(TestInsert);
      #endif
      break;
    }
    case 28: {
      #ifdef G
      RUN_TEST(TestRemove);
      #endif
      break;
    }
    case 29: {
      #ifdef G
      RUN_TEST(TestIterators);
      #endif
      break;
    }
    case 30: {
      #ifdef G
      RUN_TEST(TestConstIterators);
      #endif
      break;
    }
    case 31: {
      #ifdef G
      RUN_TEST(TestConnect);
      #endif
      break;
    }
    case 32: {
      #ifdef G
      RUN_TEST(TestDisconnect);
      #endif
      break;
    }
    case 33: {
      #ifdef G
      RUN_TEST(TestCopy);
      #endif
      break;
    }
  }
}