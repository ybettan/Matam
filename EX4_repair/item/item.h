#ifndef ITEM_H_
#define ITEM_H_

#include "exceptions.h"


namespace mtm {
namespace pokemongo {



typedef enum {CANDY,POTION} ItemType;

class Item {

		ItemType type;
		int lvl;

	public:

		// Constructs a new Item with the specified data.
		//
		// @param type the type of the item {CANDY,POTION}.
		// @param lvl the lvl of the item
		// @throw ItemInvalidArgsException if a non-positive level was sent
		Item(const ItemType type,const int lvl) : type(type),lvl(lvl)
		{	if (lvl <= 0)	throw ItemInvalidArgsException();	}


		Item(const Item& ref) = default;
		Item& operator=(const Item& item) = default;

		friend bool operator==(const Item& item1,const Item& item2)
		{	return item1.type == item2.type && item1.lvl == item2.lvl;	}

		ItemType getType() const { return type; }
		int getLvl() const { return lvl; }

		//return the ItemType of str
		// @throw ItemInvalidArgsException if str is not {"POTION","CANDY"}
		static ItemType str2itemType(std::string str) {
			if (str != "POTION" && str != "CANDY")
				throw ItemInvalidArgsException();
			return str == "POTION" ? POTION : CANDY;
		}

};


}  // namespace pokemongo
}  // namespace mtm

#endif /* ITEM_H_ */
