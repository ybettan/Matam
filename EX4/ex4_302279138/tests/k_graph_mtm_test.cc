#include "test_utils.h"
#include <iostream>
#include <string>
#include "k_graph_mtm.h"

using namespace mtm;



static bool testKGraphConstructorDestructorInsert(){
	KGraph<int,char,3> k_g('z');
	ASSERT_NO_THROW(k_g.Insert(1,'a'));
	// checks no insertion for specific value insertion.
	ASSERT_THROW(KGraphKeyAlreadyExistsExpection, k_g.Insert(1,'a'));
	ASSERT_THROW(KGraphKeyAlreadyExistsExpection, k_g.Insert(1,'b'));

	ASSERT_NO_THROW(k_g.Insert(2));//default value = 'z'

	// checks no insertion for default value insertion.
	ASSERT_THROW(KGraphKeyAlreadyExistsExpection, k_g.Insert(2));

	// checks no insertion for both insertion calls options.
	ASSERT_THROW(KGraphKeyAlreadyExistsExpection, k_g.Insert(2,'b'));
	ASSERT_THROW(KGraphKeyAlreadyExistsExpection, k_g.Insert(1));

	return true;
}

static bool testKGraphBeginAtEndIteratorAndConstIterator(){

	KGraph<int,std::string ,3> int_string_k_g1("default");
	KGraph<int,std::string ,3> int_string_k_g2("default");


	ASSERT_NO_THROW(int_string_k_g1.Insert(1,"one"));
	ASSERT_NO_THROW(int_string_k_g1.Insert(2,"two"));
	ASSERT_NO_THROW(int_string_k_g1.Insert(3,"three"));

	ASSERT_NO_THROW(int_string_k_g2.Insert(1,"one"));
	ASSERT_NO_THROW(int_string_k_g2.Insert(2,"two"));
	ASSERT_NO_THROW(int_string_k_g2.Insert(3,"three"));


	// checks beginAt and iterator operator == and != with rhs as regular
	//iterator.

	KGraph<int,std::string ,3>::iterator int_string_it11 =
			int_string_k_g1.BeginAt(1);
	KGraph<int,std::string ,3>::iterator int_string_it12 =
			int_string_k_g1.BeginAt(2);
	// next equals to int_string_it2
	KGraph<int,std::string ,3>::iterator int_string_it13 =
			int_string_k_g1.BeginAt(2);

	KGraph<int,std::string ,3>::iterator int_string_it21 =
			int_string_k_g2.BeginAt(1);
	KGraph<int,std::string ,3>::iterator int_string_it22 =
			int_string_k_g2.BeginAt(2);

	//checks beginAt exception catch.
	ASSERT_THROW(KGraphKeyNotFoundException , int_string_k_g1.BeginAt(4));
	ASSERT_THROW(KGraphKeyNotFoundException , int_string_k_g1.BeginAt(0));


	// checks beginAt const version.
	const KGraph<int, std::string, 3> const_copy_of_k_g1(int_string_k_g1);
	ASSERT_NO_THROW(const_copy_of_k_g1.BeginAt(1));
	ASSERT_THROW(KGraphKeyNotFoundException , const_copy_of_k_g1.BeginAt(4));
	ASSERT_THROW(KGraphKeyNotFoundException , const_copy_of_k_g1.BeginAt(0));



	// checks operator ==.
	ASSERT_FALSE(int_string_it11 == int_string_it12);
	ASSERT_FALSE(int_string_it11 == int_string_it21);
	ASSERT_TRUE(int_string_it12 == int_string_it13);
	//operator !=.
	ASSERT_TRUE(int_string_it11 != int_string_it12);
	ASSERT_TRUE(int_string_it11 != int_string_it21);;
	ASSERT_FALSE(int_string_it12 != int_string_it13);

	// checks iterator copy c'tor.
	KGraph<int,std::string ,3>::iterator copy_of_11(int_string_it11);
	ASSERT_TRUE(copy_of_11 == int_string_it11);

	// checks const_iterator conversion.
	KGraph<int, std::string, 3>::const_iterator const_it1(int_string_it11);

	// checks iterator operator == and != with rhs as const_iterator.
	ASSERT_TRUE(int_string_it11 == const_it1);
	ASSERT_FALSE(int_string_it12 == const_it1);
	ASSERT_TRUE(int_string_it12 != const_it1);
	ASSERT_FALSE(int_string_it11 != const_it1);

	KGraph<int,std::string ,3>::const_iterator copy_of_copy_of_11(copy_of_11);
	KGraph<int,std::string ,3>::iterator copy_of_12(int_string_it12);

	// checks const_iterator operator == and != (rhs is const_it).
	ASSERT_TRUE(copy_of_11 == copy_of_copy_of_11);
	ASSERT_FALSE(copy_of_11 == copy_of_12);
	ASSERT_TRUE(copy_of_11 != copy_of_12);
	ASSERT_FALSE(copy_of_11 != copy_of_copy_of_11);

	// checks End.
	ASSERT_FALSE(int_string_it11 == int_string_k_g1.End());
	ASSERT_TRUE(int_string_it11 != int_string_k_g1.End());

	KGraph<int,std::string ,3>::const_iterator end1 = int_string_k_g1.End();
	KGraph<int,std::string ,3>::const_iterator end11 = int_string_k_g1.End();

	KGraph<int,std::string ,3>::const_iterator end2 = int_string_k_g2.End();

	ASSERT_FALSE(end1 == end2);
	ASSERT_TRUE(end1 != end2);
	ASSERT_TRUE(end1 == end11);

	// checks iterator operator*.
	ASSERT_TRUE(*int_string_it11 == 1);
	ASSERT_TRUE(*int_string_it12 == 2);
	ASSERT_TRUE(*int_string_it12 == *int_string_it13);

	// checks const_iterator operator*.
	ASSERT_TRUE(*copy_of_12 == 2);
	ASSERT_TRUE(*copy_of_12 != 1);



	return true;
}

static bool testKGraphSubscriptOperatorContain(){

	// checks subscript operator for regular kgraph and contains.
	KGraph<int,std::string ,3> k_g("default");
	ASSERT_FALSE(k_g.Contains(1));
	ASSERT_NO_THROW(k_g.Insert(1,"first"));
	ASSERT_TRUE(k_g.Contains(1));
	ASSERT_TRUE(k_g[1] == "first");

	ASSERT_FALSE(k_g.Contains(2));
	k_g[2]; // put key=2 with default value = "default".
	ASSERT_TRUE(k_g.Contains(2));
	ASSERT_TRUE(k_g[2] == "default");
	ASSERT_FALSE(k_g[2] == "second");
	k_g[2] = "second"; // overrides the value.
	ASSERT_TRUE(k_g[2] == "second");
	ASSERT_FALSE(k_g[2] == "default");

	k_g[3] = "third";
	ASSERT_TRUE(k_g.Contains(3));
	ASSERT_TRUE(k_g[3] == "third");

	// checks subscript operator for regular kgraph and contains.
	const KGraph<int,std::string ,3> const_k_g(k_g);//const copy.
	std::string str1, str2, str3;
	ASSERT_TRUE(const_k_g.Contains(1));
	ASSERT_TRUE(const_k_g.Contains(2));
	ASSERT_TRUE(const_k_g.Contains(3));
	ASSERT_FALSE(const_k_g.Contains(0));
	ASSERT_FALSE(const_k_g.Contains(4));

	ASSERT_NO_THROW(const_k_g[1] == "first");
	ASSERT_NO_THROW(const_k_g[2] == "second");
	ASSERT_NO_THROW(const_k_g[3] == "third");
	ASSERT_THROW(KGraphKeyNotFoundException, const_k_g[0]);
	ASSERT_THROW(KGraphKeyNotFoundException, const_k_g[4]);
	str1 = k_g[1];
	str2 = k_g[2];
	str3 = k_g[3];
	ASSERT_TRUE(str1 == k_g[1]);
	ASSERT_TRUE(str2 == k_g[2]);
	ASSERT_TRUE(str3 == k_g[3]);

	return true;
}


static bool testKGraphConnectDisconnectIteratorMove(){

	//inserts 10 nodes with value a,b,c,d...

	KGraph<int,char,3> k_g('z');

	int i;
	for (i = 1; i<=10; ++i){
		ASSERT_NO_THROW(k_g.Insert(i, char(int('a') + i - 1)));
	}

	//connect in a way we can pre-order travel over the graph with index 0.

	for(i = 1; i<=9; ++i){
		ASSERT_NO_THROW(k_g.Connect(i,i+1,0,1));
	}

	// check iterator move in pre order fashion.
	i=1;
	for(KGraph<int,char,3>::iterator it = k_g.BeginAt(1); it != k_g.End();
			it.Move(0)){
		ASSERT_TRUE(*it == i);
		++i;
	}

	// checks connection of each node to itself in index 2.
	for(i=1; i<=10; ++i){
		ASSERT_NO_THROW(k_g.Connect(i,2));
	}

	// checks thorws.

	// checks KGraphKeyNotFoundException vs KGraphEdgeOutOfRange.
	ASSERT_THROW(KGraphKeyNotFoundException, k_g.Connect(0,0,3,3));
	ASSERT_THROW(KGraphKeyNotFoundException, k_g.Connect(1,0,3,3));

	// checks KGraphEdgeOutOfRange vs KGraphNodesAlreadyConnected.
	ASSERT_THROW(KGraphEdgeOutOfRange, k_g.Connect(1,1,3,3));
	ASSERT_THROW(KGraphEdgeOutOfRange, k_g.Connect(1,1,2,3));
	ASSERT_THROW(KGraphEdgeOutOfRange, k_g.Connect(1,1,2,-1));
	ASSERT_THROW(KGraphEdgeOutOfRange, k_g.Connect(1,1,2,3));

	// checks KGraphNodesAlreadyConnected vs KGraphEdgeAlreadyInUse.
	ASSERT_THROW(KGraphNodesAlreadyConnected, k_g.Connect(1,1,2,2));
	ASSERT_THROW(KGraphNodesAlreadyConnected, k_g.Connect(1,1,2,2));

	// checks KGraphEdgeAlreadyInUse.
	ASSERT_THROW(KGraphEdgeAlreadyInUse, k_g.Connect(1,5,2,2));

	// checks disconnect throws.
	// KGraphKeyNotFoundException vs kGraphNodesAreNotConnected.
	ASSERT_THROW(KGraphKeyNotFoundException, k_g.Disconnect(0,0));
	ASSERT_THROW(KGraphKeyNotFoundException, k_g.Disconnect(1,0));

	// checks kGraphNodesAreNotConnected.
	ASSERT_THROW(kGraphNodesAreNotConnected, k_g.Disconnect(1,5));

	// disconnect all self-loops.

	for(i=1; i<=10; ++i){
		ASSERT_NO_THROW(k_g.Disconnect(i,i));
		ASSERT_THROW(kGraphNodesAreNotConnected, k_g.Disconnect(i,i));
	}

	// checked ASSERT_THROW(KGraphEdgeAlreadyInUse, k_g.Connect(1,5,2,2))
	// in line 216.

	ASSERT_NO_THROW(k_g.Connect(1,5,2,2));//now its ok.


	// lets checks again copy c'tor.
	KGraph<int,char,3> k_g_copy(k_g);

	for(i = 1; i<=9; ++i){
		ASSERT_THROW(KGraphNodesAlreadyConnected, k_g_copy.Connect(i,i+1,0,1));
	}
	ASSERT_THROW(KGraphNodesAlreadyConnected, k_g_copy.Connect(1,5,2,2));

	return true;
}


static bool testKGraphRemove(){

	//inserts 10 nodes with value a,b,c,d...

	KGraph<int,char,3> k_g('z');

	int i;
	for (i = 1; i<=10; ++i){
		ASSERT_NO_THROW(k_g.Insert(i, char(int('a') + i - 1)));
	}

	ASSERT_TRUE(k_g[1] == 'a');
	ASSERT_TRUE(k_g[2] == 'b');
	ASSERT_TRUE(k_g[10] == 'j');

	//connect in a way we can pre-order travel over the graph with index 0.

	for(i = 1; i<=9; ++i){
		ASSERT_NO_THROW(k_g.Connect(i,i+1,0,1));
	}

	// before removing node with key=2 lets we catch the following throws.
	ASSERT_TRUE(k_g.Contains(2));
	ASSERT_THROW(KGraphKeyAlreadyExistsExpection, k_g.Insert(2));
	ASSERT_THROW(KGraphEdgeAlreadyInUse, k_g.Connect(1,3,0,2));

	//now we remove node with key = 2 with removing by key.
	ASSERT_NO_THROW(k_g.Remove(2));

	// trying same commands in lines 275-277 cause different behavior.
	ASSERT_FALSE(k_g.Contains(2));
	ASSERT_NO_THROW(k_g.Insert(2));
	ASSERT_NO_THROW(k_g.Connect(1,3,0,2));

	//checks KGraphKeyNotFoundException for removing by key.
	ASSERT_THROW(KGraphKeyNotFoundException, k_g.Remove(0));


	// 3 is connected to 4 via index 0.
	ASSERT_THROW(KGraphEdgeAlreadyInUse, k_g.Connect(3,5,0,1));
	KGraph<int,char,3>::iterator it1 =  k_g.BeginAt(3);
	for(; it1 != k_g.End(); it1.Move(0)){
		if(*it1 == 4) break;
	}
	ASSERT_NO_THROW(k_g.Remove(it1));

	ASSERT_NO_THROW( k_g.Connect(3,5,0,1));

	KGraph<int,char,3>::iterator it2 =  k_g.BeginAt(3);
	for(; it2 != k_g.End(); it2.Move(0)){
		if(*it2 == 4) break;
	}

	ASSERT_THROW(KGraphIteratorReachedEnd, k_g.Remove(it2));

	//inserts 10 nodes with value a,b,c,d...

	KGraph<int,char,3> k_g2('z');


	for (i = 1; i<=10; ++i){
		ASSERT_NO_THROW(k_g2.Insert(i, char(int('a') + i - 1)));
	}

	ASSERT_TRUE(k_g2[1] == 'a');
	ASSERT_TRUE(k_g2[2] == 'b');
	ASSERT_TRUE(k_g2[10] == 'j');

	//connect in a way we can pre-order travel over the graph with index 0.

	for(i = 1; i<=9; ++i){
		ASSERT_NO_THROW(k_g2.Connect(i,i+1,0,1));
	}

	KGraph<int,char,3>::iterator it3 = k_g2.BeginAt(1);
	ASSERT_NO_THROW(k_g2.Remove(it3));
	KGraph<int,char,3>::iterator it4 = k_g2.BeginAt(2);
	ASSERT_NO_THROW(k_g2.Remove(it4));
	KGraph<int,char,3>::iterator it5 = k_g2.BeginAt(3);
	ASSERT_NO_THROW(k_g2.Remove(it5));




	return true;
}



/*
int main(){
	RUN_TEST(testKGraphConstructorDestructorInsert);
	RUN_TEST(testKGraphBeginAtEndIteratorAndConstIterator);
	RUN_TEST(testKGraphSubscriptOperatorContain);
	RUN_TEST(testKGraphConnectDisconnectIteratorMove);
	RUN_TEST(testKGraphRemove);
	return 0;
}
*/
