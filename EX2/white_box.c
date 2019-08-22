#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <assert.h>
#include "pokemon.h"
#include "pokemon_trainer.h"

#define BOX

#ifdef BOX
//-----------------------------------------------------------------------------
#define TRAINER


//-----------------------------------------------------------------------------
// 							USEFULL FUNCTION
//-----------------------------------------------------------------------------

void trainerPrintElements(PokemonTrainer trainer) {
	printf("-------trainers pokemons are--------\n\n");
	for (int i=0 ; i < trainer->max_pokemons_local ; i++) {
		if (   (trainer->trainer_pokemons_array)[i] == NULL) {
			printf("pokemon_index = %d		pokemon[%d]  --->  %p\n",i+1,i,(void*)(trainer->trainer_pokemons_array)[i]);
		}else{
		printf("pokemon_index = %d		pokemon[%d]  --->  %s		rank = %d\n",i+1,i,(trainer->trainer_pokemons_array)[i]->name,pokemonGetRank(trainer->trainer_pokemons_array[i]));

		}
	}
	printf("\n\n-------proffessor pokemons are--------\n\n");
	for (int i=0 ; i < trainer->max_pokemons_remote ; i++) {
		if (   (trainer->proffessor_pokemons_array)[i] == NULL) {
			printf("pokemon_index = %d		pokemon[%d]  --->  %p\n",i+1,i,(void*)(trainer->proffessor_pokemons_array)[i]);
		}else{
		printf("pokemon_index = %d		pokemon[%d]  --->  %s		rank = %d\n",i+1,i,(trainer->proffessor_pokemons_array)[i]->name,pokemonGetRank(trainer->proffessor_pokemons_array[i]));
		}
	}
	printf("\n");
}

//-----------------------------------------------------------------------------
// 								MAIN
//-----------------------------------------------------------------------------

int main() {
//-----------------------------------------------------------------------------
#ifdef TRAINER
	printf("-------------------------------------------------------------------------\n");
	printf("			PokemonTrainerCreate test \n");
	printf("-------------------------------------------------------------------------\n");
	Pokemon pokemon = pokemonCreate("pikachu",TYPE_FIRE,80,7);
	PokemonTrainer trainer = pokemonTrainerCreate ("yoni" , pokemon ,5 , 6);
	if(trainer != NULL) {
		printf("trainer should be a valid pointer and its value is : %p \n" , (void*)trainer);
		printf("trainer.trainer_pokemons_array should be a valid pointer and its value is : %p \n", (void*)trainer->trainer_pokemons_array);
		printf("trainer.proffessor_pokemons_array should be a valid pointer and its value is : %p \n", (void*)trainer->proffessor_pokemons_array);

		if (  strcmp(trainer->name,"yoni") == 0  )
			printf("SUCCEED\n");
		else
			printf("FAILD\n");

		if (  strcmp(trainer->trainer_pokemons_array[0]->name,"pikachu") == 0  )
			printf("SUCCEED\n");
		else
			printf("FAILD\n");

		if (trainer->local_counter == 1   &&   trainer->remote_counter == 0)
			printf("SUCCEED\n");
		else
			printf("FAILD\n");


		if (trainer->max_pokemons_local ==5   &&   trainer->max_pokemons_remote == 6)
			printf("SUCCEED\n");
		else
			printf("FAILD\n");

		pokemonTrainerDestroy(trainer);

		trainer = pokemonTrainerCreate ("" , pokemon ,5 , 5);
		if (trainer == NULL)
			printf("SUCCEED\n");
		else
			printf("FAILD\n");

		trainer = pokemonTrainerCreate (NULL , pokemon ,5 , 5);
		if (trainer == NULL)
			printf("SUCCEED\n");
		else
			printf("FAILD\n");

		trainer = pokemonTrainerCreate ("yoni" , pokemon ,-1 , 5);
		if (trainer == NULL)
			printf("SUCCEED\n");
		else
			printf("FAILD\n");

		trainer = pokemonTrainerCreate ("yoni" , pokemon ,5 , 0);
		if (trainer == NULL)
			printf("SUCCEED\n");
		else
			printf("FAILD\n");

		pokemonDestroy(pokemon);

	}else{
		printf("SUCCEED simulate malloc failed\n");
	}
//-----------------------------------------------------------------------------
	printf("-------------------------------------------------------------------------\n");
	printf("			PokemonTrainerDestroy test \n");
	printf("-------------------------------------------------------------------------\n");
	trainer = pokemonTrainerCreate ("yoni" , pokemon ,5 , 5);
	if(trainer != NULL) {
		printf("trainer should be a valid pointer and its value is : %p\n",(void*)trainer);
		pokemonTrainerDestroy(trainer);
		printf("trainer should hold the same pointer than before and its value is : %p \n",(void*) trainer);
		printf("trainer->name should now be garbage and its value is : %s\n",trainer->name);
	}else{
		printf("no need to destroy cause malloc FAILED\n");
	}
//-----------------------------------------------------------------------------
	printf("-------------------------------------------------------------------------\n");
	printf("			PokemonTrainerCopy test \n");
	printf("-------------------------------------------------------------------------\n");
	pokemon = pokemonCreate("pikachu",TYPE_FIRE,80,7);
	trainer = pokemonTrainerCreate ("yoni" , pokemon ,5 , 6);

//	assert(trainer != NULL);

	PokemonTrainer trainer_copy = pokemonTrainerCopy(NULL);
	if (trainer_copy == NULL)
		printf("SUCCEED\n");
	else
		printf("FAILD\n");

	trainer_copy = pokemonTrainerCopy(trainer);
	if(trainer != NULL) {

		if (strcmp(trainer->name,trainer_copy->name) == 0)
			printf("SUCCEED\n");
		else
			printf("FAILD\n");

		if (trainer->max_pokemons_local == trainer_copy->max_pokemons_local)
			printf("SUCCEED\n");
		else
			printf("FAILD\n");

		if (trainer->max_pokemons_remote == trainer_copy->max_pokemons_remote)
			printf("SUCCEED\n");
		else
			printf("FAILD\n");

		if (trainer->local_counter == trainer_copy->local_counter)
			printf("SUCCEED\n");
		else
			printf("FAILD\n");

		if (trainer->remote_counter == trainer_copy->remote_counter)
			printf("SUCCEED\n");
		else
			printf("FAILD\n");

		printf("trainer   <--->   trainer_copy\n");
		for (int i=0 ; i<trainer->max_pokemons_local ; i++) {
			if (  trainer->trainer_pokemons_array[i] != NULL   &&   trainer_copy->trainer_pokemons_array[i])
				printf("%s   <--->   %s\n",(trainer->trainer_pokemons_array[i])->name,(trainer_copy->trainer_pokemons_array[i])->name);
		}

		for (int i=0 ; i<trainer->max_pokemons_remote ; i++) {
			if (  trainer->proffessor_pokemons_array[i] != NULL   &&   trainer_copy->proffessor_pokemons_array[i])
				printf("%s   <--->   %s\n",(trainer->proffessor_pokemons_array[i])->name,(trainer->proffessor_pokemons_array[i])->name);
		}
	}else{
		if (trainer_copy == NULL)
			printf("SUCCEED to simulate malloc failure\n");
	}
	pokemonDestroy(pokemon);
	pokemonTrainerDestroy(trainer);
	pokemonTrainerDestroy(trainer_copy);
//-----------------------------------------------------------------------------
	printf("-------------------------------------------------------------------------\n");
	printf("			pokemonTrainerAddPokemon test \n");
	printf("-------------------------------------------------------------------------\n");

	Pokemon pokemon1 = pokemonCreate("Pidgioto",TYPE_NORMAL,201,5);
	Pokemon pokemon2 = pokemonCreate("Charmander",TYPE_FIRE,101,4);
	trainer = pokemonTrainerCreate("Ash",pokemon1,2,5);
	PokemonTrainerResult result = pokemonTrainerAddPokemon(trainer,pokemon2);

	if (trainer != NULL) {
		if (result == POKEMON_TRAINER_SUCCESS)
			printf("SUCCEED\n");
		else
			printf("FAILD\n");
		printf("after creating Ash with pidgioto and adding charmander we recive : \n");
		for (int i=0 ; i<trainer->local_counter ; i++) {
			printf("----->  %s\n",(trainer->trainer_pokemons_array[i])->name);
		}
		if (trainer->local_counter == 2)
			printf("SUCCEED\n");
		else
			printf("FAILD\n");

		result = pokemonTrainerAddPokemon (NULL,pokemon1);
		if (result == POKEMON_TRAINER_NULL_ARG)
			printf("SUCCEED\n");
		else
			printf("FAILD\n");

		result = pokemonTrainerAddPokemon (trainer,NULL);
		if (result == POKEMON_TRAINER_NULL_ARG)
			printf("SUCCEED\n");
		else
			printf("FAILD\n");

		result = pokemonTrainerAddPokemon (trainer,pokemon1);
		if (result == POKEMON_TRAINER_PARTY_FULL)
			printf("SUCCEED\n");
		else
			printf("FAILD\n");
	}else if (result == POKEMON_TRAINER_NULL_ARG)
		printf("SUCCEED to simulate malloc failure\n");

	pokemonDestroy(pokemon1);
	pokemonDestroy(pokemon2);
	pokemonTrainerDestroy(trainer);
//-----------------------------------------------------------------------------
	printf("-------------------------------------------------------------------------\n");
	printf("			pokemonTrainerGetPokemon test \n");
	printf("-------------------------------------------------------------------------\n");
	pokemon1 = pokemonCreate("Pidgioto",TYPE_NORMAL,201,5);
	pokemon2 = pokemonCreate("Charmander",TYPE_FIRE,101,4);
	pokemon = pokemonCreate("pikachu",TYPE_FIRE,80,7);
	trainer = pokemonTrainerCreate("Ash",pokemon,2,5);

	if (trainer != NULL){
		result = pokemonTrainerAddPokemon(trainer,pokemon2);


		pokemon = pokemonTrainerGetPokemon(NULL,2);
		if (pokemon == NULL)
			printf("SUCCEED\n");
		else
			printf("FAILD\n");

		pokemon = pokemonTrainerGetPokemon(trainer,3);
		if (pokemon == NULL)
			printf("SUCCEED\n");
		else
			printf("FAILD\n");

		pokemon = pokemonTrainerGetPokemon(trainer,1);
		if (strcmp(pokemon->name,"pikachu") == 0) {
			printf("SUCCEED\n");
		}else{
			printf("FAILD\n");
		}
		pokemon = pokemonTrainerGetPokemon(trainer,2);
		if (strcmp(pokemon->name,"Charmander") == 0) {
			printf("SUCCEED\n");
		}else{
			printf("FAILD\n");
		}
	}

	pokemonDestroy(pokemon1);
	pokemonDestroy(pokemon2);
	pokemonDestroy(pokemon);
	pokemonTrainerDestroy(trainer);
//-----------------------------------------------------------------------------
	printf("-------------------------------------------------------------------------\n");
	printf("			pokemonTrainerRemovePokemon test \n");
	printf("-------------------------------------------------------------------------\n");

	pokemon = pokemonCreate ("Charmilion",TYPE_FIRE,50,50);
	trainer = pokemonTrainerCreate("Gery",pokemon,10,10);

	pokemon1 = NULL;
	pokemon2 = NULL;
	Pokemon pokemon3 = NULL;
	Pokemon pokemon4 = NULL;
	Pokemon pokemon5 = NULL;
	Pokemon pokemon6 = NULL;
	Pokemon pokemon7 = NULL;
	Pokemon pokemon8 = NULL;

	if (trainer != NULL){
		pokemon1 = pokemonCreate ("Squirtel",TYPE_WATER,50,50);
		pokemon2 = pokemonCreate ("Pidgioto",TYPE_NORMAL,50,50);
		pokemon3 = pokemonCreate ("Bulbasaur",TYPE_GRASS,50,50);
		pokemon4 = pokemonCreate ("Pikachu",TYPE_ELECTRIC,50,50);
		pokemon5 = pokemonCreate ("Tentacool",TYPE_WATER,50,50);
		pokemon6 = pokemonCreate ("Starmy",TYPE_WATER,50,50);
	    pokemon7 = pokemonCreate ("Electrobuz",TYPE_ELECTRIC,50,50);
		pokemon8 = pokemonCreate ("Tondebolt",TYPE_ELECTRIC,50,50);

		result = pokemonTrainerRemovePokemon(trainer,1);
		if (result == POKEMON_TRAINER_REMOVE_LAST) {
			printf("SUCCEED\n");
		}else{
			printf("FAILD\n");
		}

		pokemonTrainerAddPokemon(trainer,pokemon1);
		pokemonTrainerAddPokemon(trainer,pokemon2);
		pokemonTrainerAddPokemon(trainer,pokemon3);
		pokemonTrainerAddPokemon(trainer,pokemon4);
		pokemonTrainerAddPokemon(trainer,pokemon5);
		pokemonTrainerAddPokemon(trainer,pokemon6);
		pokemonTrainerAddPokemon(trainer,pokemon7);
		pokemonTrainerAddPokemon(trainer,pokemon8);

		trainerPrintElements(trainer);

		result = pokemonTrainerRemovePokemon(NULL,5);
		if (result == POKEMON_TRAINER_NULL_ARG) {
			printf("SUCCEED\n");
		}else{
			printf("FAILD\n");
		}

		result = pokemonTrainerRemovePokemon(trainer,20);
		if (result == POKEMON_TRAINER_INVALID_INDEX) {
			printf("SUCCEED\n");
		}else{
			printf("FAILD\n");
		}

		result = pokemonTrainerRemovePokemon(trainer,4);
		if (result == POKEMON_TRAINER_SUCCESS) {
			printf("SUCCEED\n");
		}else{
			printf("FAILD\n");
		}

		printf("after remove(trainer,4) : \n");

		trainerPrintElements(trainer);
	}

	pokemonTrainerDestroy(trainer);
	pokemonDestroy(pokemon);
	pokemonDestroy(pokemon1);
	pokemonDestroy(pokemon2);
	pokemonDestroy(pokemon3);
	pokemonDestroy(pokemon4);
	pokemonDestroy(pokemon5);
	pokemonDestroy(pokemon6);
	pokemonDestroy(pokemon7);
	pokemonDestroy(pokemon8);
//-----------------------------------------------------------------------------
printf("-------------------------------------------------------------------------\n");
printf("			pokemonTrainerDepositPokemon test \n");
printf("-------------------------------------------------------------------------\n");

pokemon = pokemonCreate ("Charmilion",TYPE_FIRE,50,50);
trainer = pokemonTrainerCreate("Gery",pokemon,10,4);

result = pokemonTrainerDepositPokemon(trainer,1);
if (result == POKEMON_TRAINER_DEPOSIT_LAST) {
	printf("SUCCEED\n\n");
}else{
	printf("FAILD\n\n");
}

pokemon1 = pokemonCreate ("Squirtel",TYPE_WATER,50,50);
pokemon2 = pokemonCreate ("Pidgioto",TYPE_NORMAL,50,50);
pokemon3 = pokemonCreate ("Bulbasaur",TYPE_GRASS,50,50);
pokemon4 = pokemonCreate ("Pikachu",TYPE_ELECTRIC,50,50);
pokemon5 = pokemonCreate ("Tentacool",TYPE_WATER,50,50);
pokemon6 = pokemonCreate ("Starmy",TYPE_WATER,50,50);
pokemon7 = pokemonCreate ("Electrobuz",TYPE_ELECTRIC,50,50);
pokemon8 = pokemonCreate ("Tondebolt",TYPE_ELECTRIC,50,50);

pokemonTrainerAddPokemon(trainer,pokemon1);
pokemonTrainerAddPokemon(trainer,pokemon2);
pokemonTrainerAddPokemon(trainer,pokemon3);
pokemonTrainerAddPokemon(trainer,pokemon4);
pokemonTrainerAddPokemon(trainer,pokemon5);
pokemonTrainerAddPokemon(trainer,pokemon6);
pokemonTrainerAddPokemon(trainer,pokemon7);
pokemonTrainerAddPokemon(trainer,pokemon8);

trainerPrintElements(trainer);

result = pokemonTrainerDepositPokemon(NULL,4);
if (result == POKEMON_TRAINER_NULL_ARG) {
	printf("SUCCEED\n");
}else{
	printf("FAILD\n");
}
result = pokemonTrainerDepositPokemon(trainer,0);
if (result == POKEMON_TRAINER_INVALID_INDEX) {
	printf("SUCCEED\n");
}else{
	printf("FAILD\n");
}

result = pokemonTrainerDepositPokemon(trainer,6);
if (result == POKEMON_TRAINER_SUCCESS) {
	printf("SUCCEED\n");
}else{
	printf("FAILD\n");
}

pokemonTrainerDepositPokemon(trainer,6);
pokemonTrainerDepositPokemon(trainer,1);
pokemonTrainerDepositPokemon(trainer,4);

result = pokemonTrainerDepositPokemon(trainer,2);
if (result == POKEMON_TRAINER_DEPOSIT_FULL) {
	printf("SUCCEED\n");
}else{
	printf("FAILD\n");
	printf("result = %d\n",result);
}

printf("\nafter using the function with indexes : 6 --> 6 --> 1 --> 4      we recive :\n");

trainerPrintElements(trainer);

pokemonTrainerDestroy(trainer);
pokemonDestroy(pokemon);
pokemonDestroy(pokemon1);
pokemonDestroy(pokemon2);
pokemonDestroy(pokemon3);
pokemonDestroy(pokemon4);
pokemonDestroy(pokemon5);
pokemonDestroy(pokemon6);
pokemonDestroy(pokemon7);
pokemonDestroy(pokemon8);
//-----------------------------------------------------------------------------
printf("-------------------------------------------------------------------------\n");
printf("			pokemonTrainerWithdrawPokemon test \n");
printf("-------------------------------------------------------------------------\n");

pokemon = pokemonCreate ("Charmilion",TYPE_FIRE,50,50);
trainer = pokemonTrainerCreate("Gery",pokemon,4,4);

pokemon1 = pokemonCreate ("Squirtel",TYPE_WATER,50,50);
pokemon2 = pokemonCreate ("Pidgioto",TYPE_NORMAL,50,50);
pokemon3 = pokemonCreate ("Bulbasaur",TYPE_GRASS,50,50);

pokemonTrainerAddPokemon(trainer,pokemon1);
pokemonTrainerAddPokemon(trainer,pokemon2);
pokemonTrainerAddPokemon(trainer,pokemon3);

pokemonTrainerDepositPokemon(trainer,1);
pokemonTrainerDepositPokemon(trainer,1);
pokemonTrainerDepositPokemon(trainer,1);

pokemon4 = pokemonCreate ("Pikachu",TYPE_ELECTRIC,50,50);
pokemon5 = pokemonCreate ("Tentacool",TYPE_WATER,50,50);
pokemon6 = pokemonCreate ("Starmy",TYPE_WATER,50,50);

pokemonTrainerAddPokemon(trainer,pokemon4);
pokemonTrainerAddPokemon(trainer,pokemon5);
pokemonTrainerAddPokemon(trainer,pokemon6);

printf("\n");
result = pokemonTrainerWithdrawPokemon(NULL,2);
if (result == POKEMON_TRAINER_NULL_ARG) {
	printf("SUCCEED\n");
}else{
	printf("FAILD\n");
	printf("result = %d\n",result);
}

result = pokemonTrainerWithdrawPokemon(trainer,5);
if (result == POKEMON_TRAINER_INVALID_INDEX) {
	printf("SUCCEED\n");
}else{
	printf("FAILD\n");
	printf("result = %d\n",result);
}

result = pokemonTrainerWithdrawPokemon(trainer,2);
if (result == POKEMON_TRAINER_PARTY_FULL) {
	printf("SUCCEED\n");
}else{
	printf("FAILD\n");
	printf("result = %d\n",result);
}

pokemonTrainerRemovePokemon(trainer,3);
pokemonTrainerRemovePokemon(trainer,3);

trainerPrintElements(trainer);

result = pokemonTrainerWithdrawPokemon(trainer,2);
if (result == POKEMON_TRAINER_SUCCESS) {
	printf("SUCCEED\n");
}else{
	printf("FAILD\n");
	printf("result = %d\n",result);
}

pokemonTrainerWithdrawPokemon(trainer,2);
printf("after using function with indexes : 2 --> 2   we recive\n");

trainerPrintElements(trainer);

pokemonTrainerDestroy(trainer);
pokemonDestroy(pokemon);
pokemonDestroy(pokemon1);
pokemonDestroy(pokemon2);
pokemonDestroy(pokemon3);
pokemonDestroy(pokemon4);
pokemonDestroy(pokemon5);
pokemonDestroy(pokemon6);
//-----------------------------------------------------------------------------
printf("-------------------------------------------------------------------------\n");
printf("			pokemonTrainerGetNumberOfPokemons test \n");
printf("-------------------------------------------------------------------------\n");

pokemon = pokemonCreate ("Charmilion",TYPE_FIRE,50,50);
trainer = pokemonTrainerCreate("Gery",pokemon,4,4);

//pokemonTrainerGetNumberOfPokemons(NULL);			//make the program stop
if (pokemonTrainerGetNumberOfPokemons(trainer) == 1) {
	printf("SUCCEED\n");
}else{
	printf("FAILD\n");
}

pokemon1 = pokemonCreate ("Squirtel",TYPE_WATER,50,50);
pokemon2 = pokemonCreate ("Pidgioto",TYPE_NORMAL,50,50);
pokemon3 = pokemonCreate ("Bulbasaur",TYPE_GRASS,50,50);

pokemonTrainerAddPokemon(trainer,pokemon1);
pokemonTrainerAddPokemon(trainer,pokemon2);
pokemonTrainerAddPokemon(trainer,pokemon3);

if (pokemonTrainerGetNumberOfPokemons(trainer) == 4) {
	printf("SUCCEED\n");
}else{
	printf("FAILD\n");
}

pokemonTrainerDepositPokemon(trainer,1);
pokemonTrainerDepositPokemon(trainer,1);

if (pokemonTrainerGetNumberOfPokemons(trainer) == 4) {
	printf("SUCCEED\n");
}else{
	printf("FAILD\n");
}

pokemonTrainerDestroy(trainer);
pokemonDestroy(pokemon);
pokemonDestroy(pokemon1);
pokemonDestroy(pokemon2);
pokemonDestroy(pokemon3);
pokemonDestroy(pokemon4);
//-----------------------------------------------------------------------------
printf("-------------------------------------------------------------------------\n");
printf("			pokemonTrainerGetMostRankedPokemon test \n");
printf("-------------------------------------------------------------------------\n");

pokemon = pokemonTrainerGetMostRankedPokemon(NULL);
if (pokemon == NULL)
	printf("SUCCEED\n");
else
	printf("FAILD\n");

pokemon = pokemonCreate ("Charmilion",TYPE_FIRE,50,50);
trainer = pokemonTrainerCreate("Gery",pokemon,4,4);

Pokemon pokemon_result = pokemonTrainerGetMostRankedPokemon(trainer);
if (  strcmp(pokemon_result->name,"Charmilion") == 0)
	printf("SUCCEED\n");
else
	printf("FAILD\n");

pokemon1 = pokemonCreate ("Squirtel",TYPE_WATER,50,50);
pokemon2 = pokemonCreate ("Pidgioto",TYPE_NORMAL,50,50);
pokemon3 = pokemonCreate ("Bulbasaur",TYPE_GRASS,50,50);

pokemonTrainerAddPokemon(trainer,pokemon1);
pokemonTrainerAddPokemon(trainer,pokemon2);
pokemonTrainerAddPokemon(trainer,pokemon3);

pokemon_result = pokemonTrainerGetMostRankedPokemon(trainer);
if (  strcmp(pokemon_result->name,"Charmilion") == 0)
	printf("SUCCEED\n");
else
	printf("FAILD\n");

pokemonTeachMove(trainer->trainer_pokemons_array[0],"fire blast",TYPE_FIRE,10,50);
pokemonTeachMove(trainer->trainer_pokemons_array[1],"wave",TYPE_WATER,10,30);
pokemonTeachMove(trainer->trainer_pokemons_array[2],"flay",TYPE_NORMAL,10,60);
pokemonTeachMove(trainer->trainer_pokemons_array[3],"leaf attack",TYPE_GRASS,10,60);

pokemon_result = pokemonTrainerGetMostRankedPokemon(trainer);
if (  strcmp(pokemon_result->name,"Pidgioto") == 0)
	printf("SUCCEED\n");
else
	printf("FAILD\n");

pokemonTrainerDepositPokemon(trainer,3);
pokemon_result = pokemonTrainerGetMostRankedPokemon(trainer);
if (  strcmp(pokemon_result->name,"Bulbasaur") == 0)
	printf("SUCCEED\n");
else
	printf("FAILD\n");

pokemonTrainerDepositPokemon(trainer,2);


	if (strcmp(pokemonTrainerGetMostRankedPokemon(trainer)->name,"Bulbasaur") == 0)
		printf("SUCCEED\n");
	else
		printf("FAILED\n");
	pokemonTrainerRemovePokemon(trainer,2);

	if (strcmp(pokemonTrainerGetMostRankedPokemon(trainer)->name,"Pidgioto") == 0)
		printf("SUCCEED\n");
	else
		printf("FAILED\n");
	pokemonTrainerWithdrawPokemon(trainer,1);
	pokemonTrainerRemovePokemon(trainer,2);

	if (strcmp(pokemonTrainerGetMostRankedPokemon(trainer)->name,"Charmilion") == 0)
		printf("SUCCEED\n");
	else
		printf("FAILED\n");
	pokemonTrainerWithdrawPokemon(trainer,1);
	pokemonTrainerRemovePokemon(trainer,1);

	if (strcmp(pokemonTrainerGetMostRankedPokemon(trainer)->name,"Squirtel") == 0)
		printf("SUCCEED\n");
	else
		printf("FAILED\n");

pokemonTrainerDestroy(trainer);
pokemonDestroy(pokemon);
pokemonDestroy(pokemon1);
pokemonDestroy(pokemon2);
pokemonDestroy(pokemon3);
//-----------------------------------------------------------------------------
printf("-------------------------------------------------------------------------\n");
printf("			pokemonTrainerMakeMostRankedParty test \n");
printf("-------------------------------------------------------------------------\n");

result = pokemonTrainerMakeMostRankedParty(NULL);
if (result == POKEMON_TRAINER_NULL_ARG)
	printf("SUCCEED\n");
else
	printf("FAILED\n");

pokemon = pokemonCreate ("Charmilion",TYPE_FIRE,50,50);
trainer = pokemonTrainerCreate("Gery",pokemon,4,4);

pokemon1 = pokemonCreate ("Squirtel",TYPE_WATER,50,50);
pokemon2 = pokemonCreate ("Pidgioto",TYPE_NORMAL,50,50);
pokemon3 = pokemonCreate ("Bulbasaur",TYPE_GRASS,50,50);
pokemon4 = pokemonCreate ("Pikachu",TYPE_ELECTRIC,50,50);
pokemon5 = pokemonCreate ("Tentacool",TYPE_WATER,50,50);

pokemonTrainerAddPokemon(trainer,pokemon1);
pokemonTrainerAddPokemon(trainer,pokemon2);
pokemonTrainerAddPokemon(trainer,pokemon3);

pokemonTeachMove(trainer->trainer_pokemons_array[0],"fire blast",TYPE_FIRE,10,100);
pokemonTeachMove(trainer->trainer_pokemons_array[1],"wave",TYPE_WATER,10,40);
pokemonTeachMove(trainer->trainer_pokemons_array[2],"flay",TYPE_NORMAL,10,20);
pokemonTeachMove(trainer->trainer_pokemons_array[3],"leaf attack",TYPE_GRASS,10,60);


pokemonTrainerDepositPokemon(trainer,1);
pokemonTrainerDepositPokemon(trainer,1);

pokemonTrainerAddPokemon(trainer,pokemon4);
pokemonTrainerAddPokemon(trainer,pokemon5);

pokemonTeachMove(trainer->trainer_pokemons_array[2],"leaf attack",TYPE_ELECTRIC,10,155);
pokemonTeachMove(trainer->trainer_pokemons_array[3],"leaf attack",TYPE_WATER,10,10);

trainerPrintElements(trainer);

result = pokemonTrainerMakeMostRankedParty(trainer);
if (result == POKEMON_TRAINER_SUCCESS)
	printf("SUCCEED\n");
else
	printf("FAILED\n");

printf("\nafter pokemonTrainerMakeMostRankedParty we recive : \n\n");
trainerPrintElements(trainer);

pokemonTrainerDestroy(trainer);
pokemonDestroy(pokemon);
pokemonDestroy(pokemon1);
pokemonDestroy(pokemon2);
pokemonDestroy(pokemon3);
pokemonDestroy(pokemon4);
pokemonDestroy(pokemon5);
//-----------------------------------------------------------------------------

	printf("-------------------------------------------------------------------------\n");
	printf("			pokemonTrainerPrintEnumeration test \n");
	printf("-------------------------------------------------------------------------\n");

	FILE* file = fopen("TrainerPrintEnumeration.txt","w");
	assert(file != NULL);

	result = pokemonTrainerPrintEnumeration(NULL,file);
	if (result == POKEMON_TRAINER_NULL_ARG)
		printf("SUCCEED\n");
	else
		printf("FAILED\n");

	result = pokemonTrainerPrintEnumeration(trainer,NULL);
	if (result == POKEMON_TRAINER_NULL_ARG)
		printf("SUCCEED\n");
	else
		printf("FAILED\n");

	pokemon = pokemonCreate ("Charmilion",TYPE_FIRE,50,50);
	trainer = pokemonTrainerCreate("Gery",pokemon,4,4);

	pokemon1 = pokemonCreate ("Squirtel",TYPE_WATER,50,50);
	pokemon2 = pokemonCreate ("Pidgioto",TYPE_NORMAL,50,50);
	pokemon3 = pokemonCreate ("Bulbasaur",TYPE_GRASS,50,50);

	pokemonTrainerAddPokemon(trainer,pokemon1);
	pokemonTrainerAddPokemon(trainer,pokemon2);
	pokemonTrainerAddPokemon(trainer,pokemon3);

//	pokemonTrainerDepositPokemon(trainer,1);
//	pokemonTrainerDepositPokemon(trainer,1);

	trainerPrintElements(trainer);

	result = pokemonTrainerPrintEnumeration(trainer,file);
	if (result == POKEMON_TRAINER_SUCCESS)
		printf("SUCCEED\n");
	else
		printf("FAILED\n");

	fclose(file);
	pokemonTrainerDestroy(trainer);
	pokemonDestroy(pokemon);
	pokemonDestroy(pokemon1);
	pokemonDestroy(pokemon2);
	pokemonDestroy(pokemon3);
printf("=========================================================================================================\n");
printf("=========================================================================================================\n");
printf("					TRAINER tests END\n");
printf("=========================================================================================================\n");
printf("=========================================================================================================\n");
}
#endif
//-----------------------------------------------------------------------------
#ifdef POKEMON
//-----------------------------------------------------------------------------
	printf("-------------------------------------------------------------------------\n");
	printf("			PokemonCreate test \n");
	printf("-------------------------------------------------------------------------\n");
	Pokemon pokemon = pokemonCreate("pickachu",TYPE_NORMAL,60,5);
	printf("pokemon should be a valid pointer and its value is : %p\n",pokemon);

	pokemon = pokemonCreate("",TYPE_NORMAL,60,5);
	if (pokemon == NULL)
		printf("SUCCEED\n");
	else
		printf("FAILD\n");

	pokemon = pokemonCreate(NULL,TYPE_NORMAL,60,5);
	if (pokemon == NULL)
		printf("SUCCEED\n");
	else
		printf("FAILD\n");

	pokemon = pokemonCreate("pikachu",-1,60,5);
	if (pokemon == NULL)
		printf("SUCCEED\n");
	else
		printf("FAILD\n");

	pokemon = pokemonCreate("pikachu",TYPE_NORMAL,9902,5);
	if (pokemon == NULL)
		printf("SUCCEED\n");
	else
		printf("FAILD\n");

	pokemon = pokemonCreate("pikachu",TYPE_NORMAL,60,-4);
	if (pokemon == NULL)
		printf("SUCCEED\n");
	else
		printf("FAILD\n");

	pokemonDestroy(pokemon);
	pokemon = pokemonCreate("pickachu",TYPE_NORMAL,60,5);

	if (strcmp(pokemon->name,"pickachu") == 0)
		printf("SUCCEED\n");
	else
		printf("FAILD\n");

	if (pokemon->type == TYPE_NORMAL)
		printf("SUCCEED\n");
	else
		printf("FAILD\n");

	if (pokemon->experience == 60)
		printf("SUCCEED\n");
	else
		printf("FAILD\n");

	if (pokemon->max_number_of_moves == 5)
		printf("SUCCEED\n");
	else
		printf("FAILD\n");

	if (pokemon->number_of_moves == 0)
		printf("SUCCEED\n");
	else
		printf("FAILD\n");

	if (pokemon->health_points == 1010)
		printf("SUCCEED\n");
	else
		printf("FAILD\n");

	printf("pokemon->moves[i] should be initiallize to NULL\n");
	for (int i=0 ; i<5 ; i++) {
		printf("pokemon->moves[%d] = %s\n",i,(pokemon->moves)[i]);
	}
	pokemonDestroy(pokemon);
//-----------------------------------------------------------------------------
	printf("-------------------------------------------------------------------------\n");
	printf("			PokemonDestroy test \n");
	printf("-------------------------------------------------------------------------\n");

	pokemon = pokemonCreate("pickachu",TYPE_NORMAL,60,5);
	printf("befor destroy pokemon point to : %p\n",pokemon);
	pokemonDestroy(pokemon);
	printf("after destroy pokemon should point so same address and its value is : %p\n",pokemon);
	printf("now all pokemon fields should point to garbage \n");
	printf("pokemon->name is : %s\n",pokemon->name);
	printf("pokemon->type is : %d\n",pokemon->type);
	printf("pokemon->experience is : %d\n",pokemon->experience);
	printf("pokemon->health_points is : %d\n",pokemon->health_points);
	printf("pokemon->number_of_moves is : %d\n",pokemon->number_of_moves);
	printf("pokemon->max_number_of_moves is : %d\n",pokemon->max_number_of_moves);

//-----------------------------------------------------------------------------
	printf("-------------------------------------------------------------------------\n");
	printf("			PokemonCopy test \n");
	printf("-------------------------------------------------------------------------\n");
	pokemon = pokemonCreate("bulbasaur",TYPE_GRASS,110,7);
	assert(pokemon != NULL);
	Pokemon pokemon_copy = pokemonCopy(pokemon);
	assert(pokemon_copy != NULL);

	if ( strcmp(pokemon->name,pokemon_copy->name) == 0 ) {
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	if (pokemon->type == pokemon_copy->type) {
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	if (pokemon->experience == pokemon_copy->experience) {
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	if (pokemon->health_points == pokemon_copy->health_points) {
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	if (pokemon->number_of_moves == pokemon_copy->number_of_moves) {
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	if (pokemon->max_number_of_moves == pokemon_copy->max_number_of_moves) {
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	int flag = 1;
	for (int i=0 ; i<pokemon_copy->max_number_of_moves ; i++) {
		if (  (pokemon->moves)[i] != (pokemon_copy->moves)[i] )
			flag = 0;
	}
	if ( flag == 1) {
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}
	pokemonDestroy(pokemon);
	pokemonDestroy(pokemon_copy);
//-----------------------------------------------------------------------------
	printf("-------------------------------------------------------------------------\n");
	printf("			pokemonTeachMove test \n");
	printf("-------------------------------------------------------------------------\n");

	pokemon = pokemonCreate("pickachu",TYPE_ELECTRIC,60,3);
	PokemonResult result = pokemonTeachMove(NULL, "fire_blast",TYPE_FIRE,6,10);
	if ( result == POKEMON_NULL_ARG) {
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	result = pokemonTeachMove(pokemon,NULL,TYPE_FIRE,6,10);
	if ( result == POKEMON_NULL_ARG) {
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	result = pokemonTeachMove(pokemon,"",TYPE_FIRE,6,10);
	if ( result == POKEMON_INVALID_MOVE_NAME) {
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	result = pokemonTeachMove(pokemon,"fire_blast",-3,6,10);
	if ( result == POKEMON_INVALID_TYPE) {
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	result = pokemonTeachMove(pokemon,"fire_blast",TYPE_FIRE,-2,10);
	if ( result == POKEMON_INVALID_POWER_POINTS) {
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	result = pokemonTeachMove(pokemon,"fire_blast",TYPE_FIRE,10,10);
	if ( result == POKEMON_INCOMPATIBLE_MOVE_TYPE) {
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	result = pokemonTeachMove(pokemon,"tonder_bolt",TYPE_ELECTRIC,10,10);
	if ( result == POKEMON_SUCCESS) {
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	result = pokemonTeachMove(pokemon,"tonder_bolt",TYPE_ELECTRIC,5,100);
	if ( result == POKEMON_MOVE_ALREADY_EXISTS) {
		printf("SUCCEED\n");
	}else{
		printf("FAILD\n");
	}

	pokemonTeachMove(pokemon,"quick_attack",TYPE_NORMAL,12,30);
	pokemonTeachMove(pokemon,"tunder",TYPE_ELECTRIC,60,60);

	printf("now pokemon moves are : \n");
	for (int i=0 ; i<3 ; i++) {
		printf("move[%d] is : %s\n",i,pokemon->moves[i]->name);
	}
	pokemonTeachMove(pokemon,"electric shock",TYPE_ELECTRIC,60,60);
	printf("after inserting new move electric shock attackes are : \n");
	for (int i=0 ; i<3 ; i++) {
		printf("move[%d] is : %s\n",i,pokemon->moves[i]->name);
	}


	pokemonDestroy(pokemon);
//-----------------------------------------------------------------------------
	printf("-------------------------------------------------------------------------\n");
	printf("			PokemonGetRank test \n");
	printf("-------------------------------------------------------------------------\n");

	//pokemonGetRank(NULL);	make the program stop

	pokemon = pokemonCreate("pickachu",TYPE_ELECTRIC,601,4);
	if (pokemonGetRank(pokemon) == 0)
		printf("SUCCEED\n");
	else
		printf("FAILED\n");

	pokemonTeachMove(pokemon,"quick attack",TYPE_NORMAL,5,20);
	if (pokemonGetRank(pokemon) == 27)
		printf("SUCCEED\n");
	else
		printf("FAILED\n");

	pokemonTeachMove(pokemon,"tonder bolt",TYPE_ELECTRIC,3,31);
	if (pokemonGetRank(pokemon) == 32)
		printf("SUCCEED\n");
	else
		printf("FAILED\n");

	pokemonTeachMove(pokemon,"tunder",TYPE_ELECTRIC,1,50);
	if (pokemonGetRank(pokemon) == 40)
		printf("SUCCEED\n");
	else
		printf("FAILED\n");

	pokemonTeachMove(pokemon,"wave",TYPE_WATER,6,15);
	if (pokemonGetRank(pokemon) == 40)
		printf("SUCCEED\n");
	else
		printf("FAILED\n");

	pokemonTeachMove(pokemon,"body slam",TYPE_NORMAL,10,18);
	if (pokemonGetRank(pokemon) == 36)
		printf("SUCCEED\n");
	else
		printf("FAILED\n");

	pokemonTeachMove(pokemon,"electro",TYPE_ELECTRIC,6,40);
	if (pokemonGetRank(pokemon) == 42)
		printf("SUCCEED\n");
	else
		printf("FAILED\n");

	pokemonDestroy(pokemon);
//-----------------------------------------------------------------------------
	printf("-------------------------------------------------------------------------\n");
	printf("			pokemonPrintName test \n");
	printf("-------------------------------------------------------------------------\n");

	pokemon = pokemonCreate("pickachu",TYPE_ELECTRIC,601,4);

	FILE* file = fopen("pokemon_name.txt","w");
	if (file == NULL) {
		printf("failed to open file");
	}

	result = pokemonPrintName(NULL,file);
	if (result == POKEMON_NULL_ARG)
		printf("SUCCEED\n");
	else
		printf("FAILED\n");

	result = pokemonPrintName(pokemon,NULL);
	if (result == POKEMON_NULL_ARG)
		printf("SUCCEED\n");
	else
		printf("FAILED\n");

	result = pokemonPrintName(pokemon,file);
	if (result == POKEMON_SUCCESS)
		printf("SUCCEED\n");
	else
		printf("FAILED\n");

	fclose(file);
	pokemonDestroy(pokemon);
//-----------------------------------------------------------------------------
	printf("-------------------------------------------------------------------------\n");
	printf("			pokemonPrintVoice test \n");
	printf("-------------------------------------------------------------------------\n");

	pokemon = pokemonCreate("pikachu",TYPE_ELECTRIC,601,4);

	file = fopen("pokemon_voice.txt","w");
	if (file == NULL) {
		printf("failed to open file");
	}

	result = pokemonPrintName(NULL,file);
	if (result == POKEMON_NULL_ARG)
		printf("SUCCEED\n");
	else
		printf("FAILED\n");

	result = pokemonPrintName(pokemon,NULL);
	if (result == POKEMON_NULL_ARG)
		printf("SUCCEED\n");
	else
		printf("FAILED\n");

	pokemonPrintVoice(pokemon,file);

	fclose(file);
	pokemonDestroy(pokemon);
//-----------------------------------------------------------------------------
}
	#endif
#endif
