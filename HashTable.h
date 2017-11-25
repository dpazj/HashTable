#ifndef MY_HASH_TABLE
#define MY_HASH_TABLE
#include "HashNode.h"
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

enum HashTableError { OUT_OF_MEMORY, KEY_NOT_FOUND, DUPLICATE_KEY}; // extend if necessary

template <class KeyType, class ValueType>
class HashTable {
  typedef vector <HashNode<KeyType,ValueType> > Table;
  Table *table; // size of table (# of buckets) is stored in the Table data structure
  size_t num;   // number of entries stored in the HashTable;

public:
  HashTable();        // constructor, initializes table of size 11;
  HashTable(size_t);  // constructor, requires size of table as arg
  ~HashTable();       // deconstructor

  size_t size();      // returns size of the hash table (number of buckets)
  size_t items(){return num;}
  size_t hash_function(KeyType);  // the table's hash function
  ValueType getValue(KeyType);    // find and return data associated with key

  void insert(KeyType,ValueType); // insert data associated with key into table
  void erase(KeyType);            // remove key and associated data from table

  void rehash(size_t); // sets a new size for the hash table, rehashes the hash table
  size_t findNextPrime(size_t currsize);
  // extend if necessary
};

template <class KeyType, class ValueType>
size_t HashTable<KeyType,ValueType>::size() {
  return table->size();
}

template <class KeyType, class ValueType>
size_t HashTable<KeyType,ValueType>::findNextPrime(size_t x) {
    x++;
    for(unsigned i = 2; i <x; i++) {
        if(x%i == 0) {
            x++;
            i=2;
        } else{
            continue;
        }
    }
    return x;
}

template <class KeyType, class ValueType>
HashTable<KeyType,ValueType>::HashTable(){
    this->num = 0;
    table = new Table;
    HashNode<KeyType, ValueType> newNode;
    for(int i=0;i<11;i++){
        try{
            table->push_back(newNode);
        }catch(const std::bad_alloc &) {
            throw OUT_OF_MEMORY;
        }


    }
}

template <class KeyType, class ValueType>
HashTable<KeyType,ValueType>::HashTable(size_t a){
    this->num = 0;
    table = new Table;
    HashNode<KeyType, ValueType> newNode;
    for(size_t i=0;i<a;i++){
        try{
            table->push_back(newNode);
        }catch(const std::bad_alloc &) {
            throw OUT_OF_MEMORY;
        }
    }
}

template <class KeyType, class ValueType>
HashTable<KeyType,ValueType>::~HashTable(){
    table->clear();
}

template <class KeyType, class ValueType>
size_t HashTable<KeyType,ValueType>::hash_function(KeyType key){
    ostringstream ss;
	ss << key;
	string keyStr = ss.str();

    unsigned int b = 378551;    //RSHASH
    unsigned int a = 63689;
    unsigned int hash = 0;

    for(std::size_t i = 0; i < keyStr.length(); i++){
       hash = hash * a + keyStr[i];
       a = a * b;
    }

	return hash % table->size();
}

template <class KeyType, class ValueType>
ValueType HashTable<KeyType,ValueType>::getValue(KeyType key){
    size_t index = hash_function(key);
    if(!table->at(index).inUse()){
        if(!table->at(index).isDeleted()){
            cout << "get val" << endl;
            cout << key << " " << index << endl;
            throw KEY_NOT_FOUND;
        }else{
            while(key != table->at(index).getKey()){
                index++;
                if(index == this->size()){
                    index = 0;
                }
            }
        }
    }else if(table->at(index).inUse() && key != table->at(index).getKey()){
        while(key != table->at(index).getKey()){
            index++;
            if(index == this->size()){
                index = 0;
            }
        }
    }
    return table->at(index).getValue();
}

template <class KeyType, class ValueType>
void HashTable<KeyType,ValueType>::insert(KeyType key, ValueType value){
    size_t index = hash_function(key);

    if(table->at(index).inUse()){
        while(table->at(index).inUse()){
            if(table->at(index).getKey() == key){
                throw DUPLICATE_KEY;
            }
            index++;
            if(index == this->size()){
                index = 0;
            }

        }
    }

    table->at(index).assign(key,value);
    this->num++;
    if((this->items()) > (this->size()*3)/4){
        try{
            rehash(findNextPrime(this->size()*2));
        }catch(HashTableError err){
            throw OUT_OF_MEMORY;
        }
    }
}

template <class KeyType, class ValueType>
void HashTable<KeyType,ValueType>::erase(KeyType key){
    size_t index = hash_function(key);
    if(!table->at(index).inUse()){
        if(!table->at(index).isDeleted()){
            throw KEY_NOT_FOUND;
        }else{
            while(key != table->at(index).getKey()){
                index++;
                if(index == this->size()){
                    index = 0;
                }
            }
        }
    }else if(table->at(index).inUse() && key != table->at(index).getKey()){
        while(key != table->at(index).getKey()){
            index++;
            if(index == this->size()){
                index = 0;
            }
        }
    }
    table->at(index).setDeleted();
    this->num--;
}

template <class KeyType, class ValueType>
void HashTable<KeyType,ValueType>::rehash(size_t size){
    this->num = 0;
    Table oldTable = *table;
    table->clear();
    HashNode<KeyType, ValueType> newNode;
    for(size_t i=0;i<size;i++){
        try{
            table->push_back(newNode);
        }catch(const std::bad_alloc &) {
            throw OUT_OF_MEMORY;
        }
    }

    for(size_t i=0;i<oldTable.size();i++){
        if(oldTable.at(i).inUse()){
            insert(oldTable.at(i).getKey(),oldTable.at(i).getValue());
        }
    }
    oldTable.clear();
}

#endif
