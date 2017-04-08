#ifndef HASH_H
#define HASH_H

#define TABLE_SIZE 65536

#include <clocale>
#include <string>
#include <stdint.h>
#include <iostream>

using namespace std;

template <class KType> class HashEntry {
    private:
          KType key;
          int value;
    public:
          HashEntry(KType key, int value);
 
          KType getKey();
          int getValue();
};

template <class KType> class HashTable {
private:
      HashEntry<KType> **table;
      int entries;
public:
      HashTable(); 

      KType getLargestValKey();

      int get(KType key);
 
      void put(KType key, int value);

      uint16_t simpleHash(int key);
      uint16_t simpleHash(string key);

      float getAvgValue();
      int getValsOverTwo();
      int getHighestValue();
 
      ~HashTable(); 
};

template <class KType> 
HashEntry<KType>::HashEntry(KType key, int value) {
    this->key = key;
    this->value = value;
}

template <class KType> 
KType HashEntry<KType>::getKey() {
    return key;
}
     
template <class KType> 
int HashEntry<KType>::getValue() {
    return value;
}
 
template <class KType>
HashTable<KType>::HashTable() {
    table = new HashEntry<KType>*[TABLE_SIZE];
    for (int i = 0; i < TABLE_SIZE; i++)
        table[i] = NULL;
}

template <class KType>
uint16_t HashTable<KType>::simpleHash(int key) {
    return key % TABLE_SIZE;
}

template <class KType>
uint16_t HashTable<KType>::simpleHash(string key) {
    uint16_t out = 5381;
    const char* c = key.c_str();

    int size = key.length();
    int bitsRead = 0;
    int bytesRead = 0;

    while (bytesRead!=size) {
        out = 33 * out ^ (unsigned char) c[bytesRead++];
    }

    return out % TABLE_SIZE;
}
 
template <class KType>
int HashTable<KType>::get(KType key) {
//    cout << " get ";
    int rot = 0;
    int hash = this->simpleHash(key);
    //cout << "h: " << hash << endl;
    while (table[hash] != NULL && table[hash]->getKey() != key) {
        hash++;
        //cout << hash << "banana" << endl;
        hash = hash % TABLE_SIZE;
    }
    if (table[hash] == NULL)
        return -1;
    else
        return table[hash]->getValue();
}
 
template <class KType>
void HashTable<KType>::put(KType key, int value) {
    //cout << " put ";
    int hash = this->simpleHash(key);
    //cout << hash << endl;
    while (table[hash] != NULL && table[hash]->getKey() != key)
        hash = (hash + 1) % TABLE_SIZE;
    if (table[hash] != NULL)
        delete table[hash];
    table[hash] = new HashEntry<KType>(key, value);
}     

template <class KType>
float HashTable<KType>::getAvgValue() {
    int val=0;
    int lines=0;
    for(int x=0; x<TABLE_SIZE; x++)
        if(table[x] != NULL && table[x]->getValue() > 2) {
            val+=table[x]->getValue();
            lines++;
        }

    return (float)val / lines;
}

template <class KType>
KType HashTable<KType>::getLargestValKey() {
    int val=0;
    int z=0;
    for(int x=0; x<TABLE_SIZE; x++)
        if(table[x] != NULL && table[x]->getValue() > val) {
            val = table[x]->getValue();
            z = x;
        }

    return table[z]->getKey();
}

template <class KType>
int HashTable<KType>::getValsOverTwo() {
    int val=0;
    for(int x=0; x<TABLE_SIZE; x++)
        if(table[x] != NULL && table[x]->getValue() > 2)
            val++;

    return val;
}
 
template <class KType>
int HashTable<KType>::getHighestValue() {
    int val=0;
    for(int x=0; x<TABLE_SIZE; x++)
        if(table[x] != NULL && table[x]->getValue() > val)
            val = table[x]->getValue();

    return val;
}
 
template <class KType>
HashTable<KType>::~HashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) 
        if (table[i] != NULL)
            delete table[i];
    delete[] table;
}

#endif
