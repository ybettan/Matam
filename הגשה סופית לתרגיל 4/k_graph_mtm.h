#ifndef K_GRAPH_MTM_H
#define K_GRAPH_MTM_H

#include "exceptions.h"
#include <iostream>
#include <map>


namespace mtm {

// Requirements: KeyType::opertor<,
//               KeyType::operator==,
//               KeyType and ValueType copy c'tor
template<typename KeyType, typename ValueType, int k>
class KGraph {

 protected:

  // A node. Represents the basic data unit in a kGraph. Has a key, a value, and
  // connected to at most k other nodes through k edges numbered from 0 to k-1.
  class Node {

  	private: 
	  KeyType key;
	  ValueType value;
	  Node** edges;

    public:
    // Constructs a new node with the given key and value.
    //
    // @param key key of the new node.
    // @param value value of the new node.
    Node(KeyType const &key, ValueType const &value);

    // A destructor.
    ~Node();

    // Returns the key of the node.
    //
    // @return the key of the node.
    KeyType const& Key() const;

    // Returns the value of the node.
    //
    // @return the value of the node.
    ValueType& Value();
    ValueType const& Value() const;

    // Returns a reference to the pointer to the neighbor node connected through
    // edge i.
    //
    // @return (reference to) a pointer to the node connected through edge i.
    const Node* const operator[](int i) const;
    Node*& operator[](int i);
  };
 public:
  class const_iterator;  // forward declaration

  // An iterator. Used to iterate over the data in a kGraph. At every given
  // moment, the iterator points either to one of the nodes in the graph, or to
  // the end of the graph.
  class iterator {
   private:
	friend class KGraph;
	friend class const_iterator;
	const KGraph* const graph;
	Node* node;


   public:
    // Constructs a new iterator that points to a given node in the given graph.
    //
    // @param node the node the new iterator points to.
    // @param graph the kGraph over which the iterator iterates.
    iterator(Node* node, KGraph* graph);

    // A copy constructor.
    //
    // @param it the iterator to copy.
    iterator(const iterator& it);

    // A destructor.
    ~iterator() = default;

    // Moves the iterator to point to the node that is connected to the current
    // node through edge i.
    //
    // @param i the edge over which to move.
    // @return a reference to *this (the same iterator) after moving it.
    // @throw KGraphEdgeOutOfRange if i is not in the range [0,k-1]
    // @throw KGraphIteratorReachedEnd when trying to move an iterator that
    //        points to the end of the graph.
    iterator& Move(int i);

    // Dereferne operator. Return the key of the node pointed by the iterator.
    //
    // @return the key of the node to which the iterator points.
    // @throw KGraphIteratorReachedEnd when trying to dereference an iterator
    //        that points to the end of the graph.
    KeyType const& operator*() const;

    // Equal operator. Two iterators are equal iff they either point to the same
    // node in the same graph, or to the end of the same graph.
    //
    // @param rhs righ hand side operand.
    // @return true iff the iterators are equal.
    bool operator==(const iterator& rhs) const;

    // Not equal operator (see definition of equality above).
    //
    // @param rhs righ hand side operand.
    // @return true iff the iterators are not equal.
    bool operator!=(const iterator& rhs) const;

    // Equal operator for a const iterator as the right-hand side operand.
    //
    // @param rhs righ hand side operand.
    // @return true iff the iterators are equal.
    bool operator==(const const_iterator& rhs) const;

    // Not equal operator for a const iterator as the right-hand side operand.
    //
    // @param rhs righ hand side operand.
    // @return true iff the iterators are not equal.
    bool operator!=(const const_iterator& rhs) const;
  };

  // A const iterator. Used to iterate over the data in a constant kGraph.
  // Similarly to a regular iterator, at every given moment, the iterator points
  // either to one of the nodes in the graph, or to the end of the graph.
  class const_iterator {
  private:
	friend class KGraph;
	friend class iterator;
	const KGraph* const graph;
    const Node* node;



   public:
    // Constructs a new const iterator that points to a given node in the given
    // graph.
    //
    // @param node the node the new iterator points to.
    // @param graph the kGraph over which the iterator iterates.
    const_iterator(const Node* node, const KGraph* graph);

    // A copy constructor.
    //
    // @param it the iterator to copy.
    const_iterator(const const_iterator& it);

    // Conversion from a regular iterator. Constucts a new const iterator that
    // points to the same node as the given iterator.
    //
    // @param it the iterator we would like to convert to const iterator.
    const_iterator(const iterator& it);

    // A destructor.
    ~const_iterator() = default;

    // Moves the iterator to point to the node that is connected to the current
    // node through edge i.
    //
    // @param i the edge over which to move.
    // @return a reference to *this (the same iterator) after moving it.
    // @throw KGraphEdgeOutOfRange if i is not in the range [0,k-1]
    // @throw KGraphIteratorReachedEnd when trying to move an iterator that
    //        points to the end of the graph.
    const_iterator& Move(int i);

    // Dereferne operator. Return the key of the node pointed by the iterator.
    //
    // @return the key of the node to which the iterator points.
    // @throw KGraphIteratorReachedEnd when trying to dereference an iterator
    //        that points to the end of the graph.
    KeyType const& operator*() const;

    // Equal operator. Two iterators are equal iff they either point to the same
    // node in the same graph, or to the end of the same graph.
    //
    // @param rhs righ hand side operand.
    // @return true iff the iterators are equal.
    bool operator==(const const_iterator& rhs) const;

    // Not equal operator (see definition of equality above).
    //
    // @param rhs righ hand side operand.
    // @return true iff the iterators are not equal.
    bool operator!=(const const_iterator& rhs) const;
  };

 private:

  class keyCompare{
  public:


	  bool operator()(const KeyType& lhs, const KeyType& rhs) {
		  return lhs < rhs;
	  }
	  bool operator()(const KeyType& lhs, const KeyType& rhs) const{
		  return lhs < rhs;
	  }
  };

  std::map<KeyType, Node*, keyCompare> map; ///should it be private?? and if so-problem:
  ValueType const default_value;             // how to get other kg default when copying?




 public:
  // Constructs a new empty kGraph with the given default value.
  //
  // @param default_value the default value in the graph.
  explicit KGraph(ValueType const& default_value);

  // A copy constructor. Copies the given graph. The constructed graph will have
  // the exact same structure with copied data.
  //
  // @param k_graph the graph to copy.
  KGraph(const KGraph& k_graph);

  // A destructor. Destroys the graph together with all resources allocated.
  ~KGraph();

  // Returns an iterator to the node with the given key.
  //
  // @param i the key of the node which the returned iterator points to.
  // @return iterator the newly constructed iterator.
  // @throw KGraphKeyNotFoundException when the given key is not found in the
  //        graph.
  iterator BeginAt(KeyType const& i);
  const_iterator BeginAt(KeyType const& i) const;

  // Returns an iterator to the end of the graph.
  //
  // @return iterator an iterator to the end of the graph.
  const_iterator End() const;

  // Inserts a new node with the given data to the graph.
  //
  // @param key the key to be assigned to the new node.
  // @param value the value to be assigned to the new node.
  // @throw KGraphKeyAlreadyExistsExpection when trying to insert a node with a
  //        key that already exists in the graph.
  void Insert(KeyType const& key, ValueType const& value);

  // Inserts a new node with the given key and the default value to the graph.
  //
  // @param key the key to be assigned to the new node.
  // @throw KGraphKeyAlreadyExistsExpection when trying to insert a node with a
  //        key that already exists in the graph.
  void Insert(KeyType const& key);

  // Removes the node with the given key from the graph.
  //
  // @param key the key of the node to be removed.
  // @throw KGraphKeyNotFoundException when trying to remove a key that cannot
  //        be found in the graph.
  void Remove(KeyType const& key);

  // Removes the node pointed by the given iterator from the graph. If the
  // given iterator neither points to a node in this graph nor to the end of
  // this graph, the behaviour of this function is undefined.
  //
  // @param it the iterator that points to the node to be removed.
  // @throw KGraphIteratorReachedEnd when the given iterator points to the end
  //        of the graph.
  void Remove(const iterator& it);

  // The subscript operator. Returns a reference to the value assigned to
  // the given key in the graph. If the key does not exist, inserts a new node
  // to the graph with the given key and the default value, then returns a
  // refernce to its value.
  //
  // @param key the key to return its value.
  // @return the value assigned to the given key.
  ValueType& operator[](KeyType const& key);

  // A const version of the subscript operator. Returns the value assigned to
  // the given key in the graph. If the key does not exist, throws an exception.
  //
  // @param key the key to return its value.
  // @return the value assigned to the given key.
  // @throw KGraphKeyNotFoundException if the given key cannot be found in the
  //        graph.
  ValueType const& operator[](KeyType const& key) const;

  // Checks whether the graph contains the given key.
  //
  // @param key
  // @return true iff the graph contains the given key.
  bool Contains(KeyType const& key) const;

  // Connects two nodes in the graph with an edge.
  //
  // @param key_u the key of the first node.
  // @param key_v the key of the second node.
  // @param i_u the index of the new edge at the first node.
  // @param i_v the index of the new edge at the second node.
  // @throw KGraphKeyNotFoundException if at least one of the given keys cannot
  //        be found in the graph.
  // @throw KGraphEdgeOutOfRange if i is not in the range [0,k-1].
  // @throw KGraphNodesAlreadyConnected if the two nodes are already connected.
  // @throw KGraphEdgeAlreadyInUse if at least one of the indices of the edge at
  //        one of the nodes is already in use.
  void Connect(KeyType const& key_u, KeyType const& key_v, int i_u, int i_v);

  // Connects a node to itself via a self loop.
  //
  // @param key the key of the node.
  // @param i the index of the self loop.
  // @throw KGraphKeyNotFoundException if the given keys cannot be found in the
  //        graph.
  // @throw KGraphEdgeOutOfRange if i is not in the range [0,k-1]
  // @throw KGraphNodesAlreadyConnected if the node is already self connected.
  // @throw KGraphEdgeAlreadyInUse if the index of the self loop is already in
  //        use.
  void Connect(KeyType const& key, int i);

  // Disconnects two connected nodes.
  //
  // @param key_u the key of the first node.
  // @param key_v the key of the second node.
  // @throw KGraphKeyNotFoundException if at least one of the given keys cannot
  //        be found in the graph.
  // @throw kGraphNodesAreNotConnected if the two nodes are not connected.
  void Disconnect(KeyType const& key_u, KeyType const& key_v);
};

/*
 * node templates.
 */

template<typename KeyType, typename ValueType, int k>
KGraph<KeyType,ValueType,k>::Node::Node(KeyType const &key,
		ValueType const &value) : key(key), value(value), edges(new Node*[k]){

	for(int i=0; i<k; ++i){
		edges[i] = NULL;
	}
}

template<typename KeyType, typename ValueType, int k>
KGraph<KeyType,ValueType,k>::Node::~Node(){

	delete[] edges;
}

template<typename KeyType, typename ValueType, int k>
KeyType const& KGraph<KeyType,ValueType,k>::Node::Key() const{

	return this->key;
}

template<typename KeyType, typename ValueType, int k>
ValueType& KGraph<KeyType,ValueType,k>::Node::Value(){

	return this->value;
}

template<typename KeyType, typename ValueType, int k>
ValueType const& KGraph<KeyType,ValueType,k>::Node::Value() const{

	return this->value;
}

template<typename KeyType, typename ValueType, int k>
typename KGraph<KeyType,ValueType,k>::Node*&
		KGraph<KeyType,ValueType,k>::Node::operator[](int i){

	return this->edges[i];
}

template<typename KeyType, typename ValueType, int k>
const typename KGraph<KeyType,ValueType,k>::Node* const
		KGraph<KeyType,ValueType,k>::Node::operator[](int i) const{

	return this->edges[i];
}
/*
 * iterator templates
 */

template<typename KeyType, typename ValueType, int k>
KGraph<KeyType,ValueType,k>::iterator::iterator(typename KGraph<KeyType,
		ValueType,k>::Node* node_ptr,  KGraph* k_graph) :
		graph(k_graph), node(node_ptr){}

template<typename KeyType, typename ValueType,int k>
KGraph<KeyType,ValueType,k>::iterator::iterator(const typename KGraph<KeyType,
		ValueType, k>::iterator& it): graph(it.graph), node(it.node){}

template<typename KeyType, typename ValueType, int k>
typename KGraph<KeyType, ValueType, k>::iterator&
		KGraph<KeyType,ValueType,k>::iterator::Move(int i){

	if(i<0 || i> k-1) throw KGraphEdgeOutOfRange();
	if(this->node == NULL) throw KGraphIteratorReachedEnd();
	this->node = (*(this->node))[i];
	return *this;
}

template<typename KeyType, typename ValueType, int k>
const KeyType& KGraph<KeyType,ValueType,k>::iterator::operator *() const{

	if(this->node == NULL) throw KGraphIteratorReachedEnd();
	const KGraph::Node* node_ptr = this->node;
	return node_ptr->Key();
}

template<typename KeyType, typename ValueType, int k>
bool KGraph<KeyType,ValueType,k>::iterator::operator ==(const typename
		KGraph<KeyType, ValueType, k>::iterator& rhs) const{

	if(this->graph != rhs.graph || this->node != rhs.node) return false;
	return true;
}

template<typename KeyType, typename ValueType, int k>
bool KGraph<KeyType,ValueType,k>::iterator::operator!=(const
		iterator& rhs) const{

	return !(*this == rhs);
}

template<typename KeyType, typename ValueType, int k>
bool KGraph<KeyType, ValueType, k>::iterator::operator==(const KGraph<KeyType,
		ValueType, k>::const_iterator& rhs) const{

	if(this->graph != rhs.graph || this->node != rhs.node) return false;
	return true;
}

template<typename KeyType, typename ValueType, int k>
bool KGraph<KeyType, ValueType, k>::iterator::operator!=(const KGraph<KeyType,
		ValueType, k>::const_iterator& rhs) const{

	return !(*this == rhs);
}

/*
 * const_iterator templates
 */

template<typename KeyType, typename ValueType, int k>
KGraph<KeyType,ValueType,k>::const_iterator::const_iterator(const typename
		KGraph<KeyType,ValueType,k>::Node* node,
		const KGraph<KeyType, ValueType ,k>* k_graph):
		graph(k_graph),	node(node){}

template<typename KeyType, typename ValueType, int k>
KGraph<KeyType,ValueType,k>::const_iterator::const_iterator(const
		KGraph<KeyType,ValueType, k>::const_iterator& it):
		graph(it.graph), node(it.node){}

template<typename KeyType, typename ValueType, int k>
KGraph<KeyType,ValueType,k>::const_iterator::const_iterator(const
		KGraph<KeyType,ValueType,k>::iterator& it):
		graph(it.graph), node(it.node){}

template<typename KeyType, typename ValueType, int k>
typename KGraph<KeyType, ValueType, k>::const_iterator& KGraph<KeyType,
ValueType, k>::const_iterator::Move(int i){

	if(i<0 || i<k-1) throw KGraphEdgeOutOfRange();
	if(this->node == NULL) throw KGraphIteratorReachedEnd();
	this->node = (*(this->node))[i];
	return *this;
}

template<typename KeyType, typename ValueType, int k>
KeyType const& KGraph<KeyType,ValueType,k>::const_iterator::operator *() const{

	if(this->node == NULL) throw KGraphIteratorReachedEnd();
	return this->node->Key();
}

template<typename KeyType, typename ValueType, int k>
bool KGraph<KeyType, ValueType, k>::const_iterator::operator ==(const
		KGraph<KeyType,ValueType, k>::const_iterator& rhs) const{

	if(this->graph != rhs.graph || this->node != rhs.node) return false;
	return true;
}

template<typename KeyType, typename ValueType, int k>
bool KGraph<KeyType,ValueType,k>::const_iterator::operator !=(const
		KGraph<KeyType,ValueType,k>::const_iterator& rhs) const{

	return !(*this == rhs);
}

/*
 * KGraph templates
 */

template<typename KeyType, typename ValueType, int k>
KGraph<KeyType, ValueType, k>::KGraph(ValueType const& default_value):
		default_value(default_value){}

template<typename KeyType, typename ValueType, int k>
KGraph<KeyType, ValueType, k>::KGraph(const KGraph& k_graph):
		default_value(k_graph.default_value){

	KGraph::Node* node_ptr;
	//next section inserts new node matchin to original in the map.
	typename std::map<KeyType,KGraph::Node*>::const_iterator map_it =
			k_graph.map.begin();
	while(map_it != k_graph.map.end()){
		node_ptr = map_it->second;
		(this->map)[map_it->first] =
				new KGraph::Node(map_it->first,node_ptr->Value());
		++map_it;
	}

	map_it = this->map.begin();
	//next section copies the edges connection.
	typename std::map<KeyType,KGraph::Node*>::const_iterator const_it;
	KeyType key_to_connect;
	while(map_it != this->map.end()){
		const_it = k_graph.map.find(map_it->first);
		node_ptr = const_it->second;
		for(int i = 0; i<k; ++i){
			if( (*node_ptr)[i] == NULL ) continue;//no edges.
			key_to_connect = ((*node_ptr)[i])->Key();//the key of the node to
			                                         //connect.
			(*(map_it->second))[i] = (this->map)[key_to_connect];
		}
		++map_it;
	}
}


template<typename KeyType, typename ValueType, int k>
KGraph<KeyType, ValueType, k>::~KGraph(){

	typename std::map<KeyType, KGraph::Node*>::iterator map_it =
				this->map.begin();
	while(map_it != this->map.end()){
		delete map_it->second;
		++map_it;
	}
}

template<typename KeyType, typename ValueType, int k>
typename KGraph<KeyType, ValueType, k>::iterator
		KGraph<KeyType,ValueType,k>::BeginAt(const KeyType &i){

	if(this->map.find(i) == this->map.end())
		throw KGraphKeyNotFoundException();
	KGraph::iterator k_g_it((this->map)[i], this);
	return k_g_it;
}

template<typename KeyType, typename ValueType, int k>
typename KGraph<KeyType,ValueType,k>::const_iterator KGraph<KeyType,
		ValueType,k>::BeginAt(const KeyType &i) const{

	if(this->map.find(i) == this->map.end())
		throw KGraphKeyNotFoundException();
	return KGraph::const_iterator(this->map.at(i),this);
}


template<typename KeyType, typename ValueType, int k>
typename KGraph<KeyType,ValueType,k>::const_iterator
		KGraph<KeyType,ValueType,k>::End() const{

	return KGraph::const_iterator(NULL, this);
}

template<typename KeyType, typename ValueType, int k>
void KGraph<KeyType,ValueType,k>::Insert(KeyType const& key,
		ValueType const& value){

	if(this->map.find(key) != this->map.end())
		throw KGraphKeyAlreadyExistsExpection();
	KGraph::Node* node_ptr = new KGraph::Node(key, value);
	this->map.insert(std::pair<KeyType, KGraph::Node*>(key, node_ptr));
}

template<typename KeyType, typename ValueType, int k>
void KGraph<KeyType,ValueType,k>::Insert(KeyType const& key){

	if(this->map.find(key) != this->map.end())
		throw KGraphKeyAlreadyExistsExpection();
	KGraph::Node* node_ptr = new KGraph::Node(key, this->default_value);
	this->map.insert(std::pair<KeyType, KGraph::Node*>(key, node_ptr));
}

template<typename KeyType, typename ValueType, int k>
void KGraph<KeyType,ValueType,k>::Remove(KeyType const& key){

	if(this->map.find(key) == this->map.end())
		throw KGraphKeyNotFoundException();
	KGraph::Node* node_to_remove_ptr = this->map[key];
	KGraph::Node* node_to_disconnect_ptr;
	for(int i=0; i<k; ++i){
		node_to_disconnect_ptr = (*node_to_remove_ptr)[i];
		if(node_to_disconnect_ptr){
			this->Disconnect(node_to_disconnect_ptr->Key(),
					node_to_remove_ptr->Key());
		}
	}
	this->map.erase(key);
	delete(node_to_remove_ptr);
}

template<typename KeyType, typename ValueType, int k>
void KGraph<KeyType,ValueType,k>::Remove(const iterator& it){

	this->Remove(*it);
}



template<typename KeyType, typename ValueType, int k>
ValueType& KGraph<KeyType,ValueType,k>::operator [](KeyType const& key){

	if(this->Contains(key) == false){
		this->Insert(key);
	}
	KGraph::Node* node_ptr = (this->map)[key];
	return node_ptr->Value();
}

template<typename KeyType, typename ValueType, int k>
ValueType const& KGraph<KeyType,ValueType,k>::operator [](KeyType
		const& key) const{

	KGraph::Node* node_ptr;
	try{
		node_ptr = this->map.at(key);
	} catch (...){
		throw KGraphKeyNotFoundException();
	}
	return node_ptr->Value();
}

template<typename KeyType, typename ValueType, int k>
bool KGraph<KeyType,ValueType,k>::Contains(KeyType const& key) const{

	try{
		this->BeginAt(key);
	} catch (...) {
		return false;
	}
	return true;
	}


template<typename KeyType, typename ValueType, int k>
void KGraph<KeyType,ValueType,k>::Connect(KeyType const& key_u,
		KeyType const& key_v, int i_u, int i_v){

	KGraph::Node* u;
	KGraph::Node* v;
	try{
		u = this->map.at(key_u);
		v = this->map.at(key_v);
	} catch (...) {
		throw KGraphKeyNotFoundException();
	}
	if(i_u < 0 || i_u > k-1 || i_v < 0 || i_v > k-1)
		throw KGraphEdgeOutOfRange();

	for(int i = 0; i < k; ++i){
		if((*u)[i] == NULL) continue;
		if((*u)[i]->Key() == v->Key()) throw KGraphNodesAlreadyConnected();
	}
	if((*u)[i_u] != NULL || (*v)[i_v] != NULL) throw KGraphEdgeAlreadyInUse();
	(*u)[i_u] = v;
	(*v)[i_v] = u;
}

template<typename KeyType, typename ValueType, int k>
void KGraph<KeyType,ValueType,k>::Connect(KeyType const& key, int i){

	this->Connect(key, key, i, i);
}

template<typename KeyType, typename ValueType, int k>
void KGraph<KeyType,ValueType,k>::Disconnect(KeyType const& key_u,
		KeyType const& key_v){

	KGraph::Node* u;
	KGraph::Node* v;
	try{
		u = (this->map.at(key_u));
		v = (this->map.at(key_v));
	} catch (...) {// getting first index.
		throw KGraphKeyNotFoundException();
	}
	int i,j;
	for(i = 0; i < k; ++i){
		if((*u)[i] == NULL) continue;
		if((*u)[i]->Key() == v->Key()) break;// getting first index.
	}
	for(j = 0; j < k; ++j){
		if((*v)[j] == NULL) continue;
		if((*v)[j]->Key() == u->Key()) break;// getting second index.
	}
	if(i >= k || j >= k) throw kGraphNodesAreNotConnected();
	(*u)[i] = NULL;
	(*v)[j] = NULL;
}


}  // namespace mtm
#endif  // K_GRAPH_MTM_H
