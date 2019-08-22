CC = gcc 
COMP_FLAG = -std=c99 -Wall -pedantic -errors -Werror
LIB = -L. -lmtm -lm

EXEC1 = pokemon_go_mtm
EXEC2 = tests
TESTS = tests/map_mtm_test tests/stock_test tests/store_test tests/pokemon_test tests/trainer_test \
	tests/pokemonGo_test tests/parcer_test

MAP_OBJS = map_mtm.o
MAP_TEST_OBJS = map_mtm_test.o
STOCK_OBJS = stock.o  
STOCK_TEST_OBJS = stock_test.o 
STORE_OBJS = store.o 
STORE_TEST_OBJS = store_test.o 
POKEMON_OBJS = pokemon.o 
POKEMON_TEST_OBJS = pokemon_test.o 
TRAINER_OBJS = trainer.o 
TRAINER_TEST_OBJS = trainer_test.o 
POKEMONGO_OBJS = pokemonGo.o  
POKEMONGO_TEST_OBJS = pokemonGo_test.o 
PARCER_OBJS = parcer.o
PARCER_TEST_OBJS = parcer_test.o
MAIN_OBJS = main.o
MTM_OBJS = libmtm.a

ALL_OBJS = $(MAIN_OBJS) $(PARCER_OBJS) $(POKEMONGO_OBJS) $(TRAINER_OBJS) $(POKEMON_OBJS) $(STORE_OBJS) $(STOCK_OBJS)  
ALL_TESTS_OBJS = $(PARCER_TEST_OBJS) $(POKEMONGO_TEST_OBJS) $(TRAINER_TEST_OBJS) $(POKEMON_TEST_OBJS) $(STORE_TEST_OBJS) \
		 $(STOCK_TEST_OBJS)

#-------------------------------------------------------------------------------------

$(EXEC1) : $(ALL_OBJS) $(MTM_OBJS)
	$(CC) $(ALL_OBJS) $(MTM_OBJS) -o $@

$(MAIN_OBJS) : main.c print_utils.h parcer.h pokemonGo.h
	$(CC) -c $(COMP_FLAG) $(LIB) $*.c 
$(POKEMONGO_OBJS) : pokemonGo.c map.h set.h list.h pokemonGo.h trainer.h store.h  
	$(CC) -c $(COMP_FLAG) $(LIB) $*.c 
$(PARCER_OBJS) : parcer.c parcer.h pokemonGo.h
	$(CC) -c $(COMP_FLAG) $(LIB) $*.c 
$(TRAINER_OBJS) : trainer.c trainer.h pokemon.h stock.h print_utils.h map.h 
	$(CC) -c $(COMP_FLAG) $(LIB) $*.c 
$(POKEMON_OBJS) : pokemon.c pokemon.h print_utils.h
	$(CC) -c $(COMP_FLAG) $(LIB) $*.c 
$(STORE_OBJS) : store.c store.h stock.h 
	$(CC) -c $(COMP_FLAG) $(LIB) $*.c 
$(STOCK_OBJS) : stock.c print_utils.h map.h stock.h 
	$(CC) -c $(COMP_FLAG) $(LIB) $*.c 

#-------------------------------------------------------------------------------------

$(EXEC2) : $(ALL_OBJS) $(ALL_TESTS_OBJS) $(MAP_OBJS) $(MAP_TEST_OBJS) $(MTM_OBJS) 
	$(CC) $(LIB) $(MAP_OBJS) $(MAP_TEST_OBJS) $(MTM_OBJS) -o tests/map_mtm_test
	$(CC) $(LIB) $(STOCK_OBJS) $(STOCK_TEST_OBJS) $(MTM_OBJS) -o tests/stock_test
	$(CC) $(LIB) $(STOCK_OBJS) $(STORE_OBJS) $(STORE_TEST_OBJS) $(MTM_OBJS) -o tests/store_test
	$(CC) $(LIB) $(POKEMON_OBJS) $(POKEMON_TEST_OBJS) $(MTM_OBJS) -o tests/pokemon_test 
	$(CC) $(LIB) $(POKEMON_OBJS) $(STOCK_OBJS) $(TRAINER_OBJS) $(TRAINER_TEST_OBJS) $(MTM_OBJS) -o tests/trainer_test
	$(CC) $(LIB) $(POKEMON_OBJS) $(TRAINER_OBJS) $(STOCK_OBJS) $(STORE_OBJS) $(POKEMONGO_OBJS) \
								$(POKEMONGO_TEST_OBJS) $(MTM_OBJS) -o tests/pokemonGo_test
	$(CC) $(LIB) $(POKEMON_OBJS) $(TRAINER_OBJS) $(STOCK_OBJS) $(STORE_OBJS) $(POKEMONGO_OBJS) \
							$(PARCER_OBJS) $(PARCER_TEST_OBJS) $(MTM_OBJS) -o tests/parcer_test
	

$(MAP_OBJS) : map_mtm/map_mtm.c map.h 
	$(CC) -c $(COMP_FLAG) $(LIB) map_mtm/$*.c 

$(MAP_TEST_OBJS) : tests/map_mtm_test.c map.h test_utilities.h
	$(CC) -c $(COMP_FLAG) $(LIB) tests/$*.c 

$(STOCK_TEST_OBJS) : tests/stock_test.c tests/../print_utils.h tests/../yoni_aux_macros.h tests/../stock.h
	$(CC) -c $(COMP_FLAG) $(LIB) tests/$*.c 

$(STORE_TEST_OBJS) : tests/store_test.c tests/../yoni_aux_macros.h tests/../store.h tests/../stock.h
	$(CC) -c $(COMP_FLAG) $(LIB) tests/$*.c 

$(POKEMON_TEST_OBJS) : tests/pokemon_test.c tests/../test_utilities.h tests/../pokemon.h
	$(CC) -c $(COMP_FLAG) $(LIB) tests/$*.c 

$(TRAINER_TEST_OBJS) : tests/trainer_test.c tests/../test_utilities.h tests/../trainer.h
	$(CC) -c $(COMP_FLAG) $(LIB) tests/$*.c 

$(POKEMONGO_TEST_OBJS) : tests/pokemonGo_test.c tests/../test_utilities.h tests/../pokemonGo.h
	$(CC) -c $(COMP_FLAG) $(LIB) tests/$*.c 

$(PARCER_TEST_OBJS) : tests/parcer_test.c tests/../test_utilities.h tests/../parcer.h tests/../pokemonGo.h
	$(CC) -c $(COMP_FLAG) $(LIB) tests/$*.c 

#-----------------------------------------------------------------------------------------

clean:  
	rm -f $(MAP_OBJS) $(MAP_TEST_OBJS) $(STOCK_OBJS) $(STOCK_TEST_OBJS)$(STORE_OBJS) $(STORE_TEST_OBJS) $(POKEMON_OBJS) \
	$(POKEMON_TEST_OBJS) $(TRAINER_OBJS) $(TRAINER_TEST_OBJS) $(POKEMONGO_OBJS) $(POKEMONGO_TEST_OBJS) $(PARCER_OBJS) \
	$(PARCER_TEST_OBJS) $(MAIN_OBJS) $(EXEC1) $(TESTS) 
