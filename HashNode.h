#ifndef MY_HASH_NODE
#define MY_HASH_NODE

#include <iostream>
#include <string>
#include <typeinfo>

using namespace std;

template <class KeyType, class ValueType>
class HashNode {
  KeyType key;     // The hash node's key
  ValueType value; // The key's associated data
  bool used;
  bool deleted;
  /* extend if necessary */
public:

  HashNode();  // constructor
  KeyType getKey() { return key; }
  ValueType getValue() { return value; }
  bool inUse(){return used;}
  bool isDeleted(){return deleted;}
  void assign(KeyType key, ValueType value);
  void setDeleted();
};

template <class KeyType, class ValueType>
HashNode<KeyType,ValueType>::HashNode(){
    this->used = false;
    this->deleted = false;
    // setEmptyValue();
}

template <class KeyType, class ValueType>
void HashNode<KeyType,ValueType>::assign(KeyType key, ValueType value){
    this->used = true;
    this->deleted = false;
    this->key = key;
    this->value = value;
}

template <class KeyType, class ValueType>
void HashNode<KeyType,ValueType>::setDeleted(){
    this->used = false;
    this->deleted = true;
}


#endif
