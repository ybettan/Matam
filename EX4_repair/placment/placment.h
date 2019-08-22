#ifndef PLACMENT_H_
#define PLACMENT_H_

#include <string>

#include "trainer.h"

using std::string;

namespace mtm {
namespace pokemongo {


class Placment {

	string location_name;
	Trainer* trainer;

public:

	/* a creatore which recive a location name and a trainer pointer
	 * throw - PlacmentInvalidArgsException if trainer == NULL
	 */
	Placment(const string location_name,Trainer* trainer) :
		location_name(location_name) , trainer(trainer)
		{	if (trainer == NULL)	throw PlacmentInvalidArgsException();	}

	Placment(const Placment& ref) = delete;
	Placment& operator=(const Placment& placment) = delete;

	//return the location name wich in trainer is located
	const string& GetLocationName() const
		{	return location_name;	}

	//return the trainer of a certain placment
	Trainer* GetTrainer()
		{	return trainer;	}

};



}  // namespace pokemongo
}  // namespace mtm

#endif /* PLACMENT_H_ */
