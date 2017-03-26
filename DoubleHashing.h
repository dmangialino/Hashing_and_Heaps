// Dina Mangialino
// CSCI 335 Assignment 3 - Hashing and Heaps
// March 31, 2016

// DoubleHashing.h
// Header file for class that implements hash table with double hashing.

// Most of the methods are the same as the provided Quadratic Probing
// Changes made to FindPos()
// Collision and probe counts added
// CurrentSize(), TableSize(), and CollisionCount() public methods added
// SecondHash() private method added

#ifndef CSCI335_HASHING_DOUBLE_HASHING_H_
#define CSCI335_HASHING_DOUBLE_HASHING_H_

#include <iostream>
#include <vector>
#include <string>
using namespace std;

template <typename HashedObj>
class DoubleHashTable {
 public:
  enum EntryType {kActive, kEmpty, kDeleted};

  explicit DoubleHashTable(int size = 101) : array_(NextPrime(size))
    { MakeEmpty(); }
  DoubleHashTable(const DoubleHashTable &a_hash_table) = default;
  DoubleHashTable(DoubleHashTable &&a_hash_table) = default;
  DoubleHashTable &operator=(const DoubleHashTable &a_hash_table) = default;
  DoubleHashTable &operator=(DoubleHashTable &&a_hash_table) = default;
  ~DoubleHashTable() = default;

  bool Contains(const HashedObj & x, int & probes) const;
  void MakeEmpty();
  bool Insert(const HashedObj & x);
  bool Insert(HashedObj && x);
  bool Remove(const HashedObj & x);
	
	// new methods
  int CurrentSize();
  int TableSize();
	int CollisionCount();

 private:
  struct HashEntry {
  HashedObj element_;
  EntryType info_;
    
  HashEntry(const HashedObj &element = HashedObj{}, EntryType infotype = kEmpty)
   : element_{element}, info_{infotype} {}    
  HashEntry(HashedObj &&element, EntryType infotype = kEmpty)
   : element_{std::move(element)}, info_{infotype} { }
 };

  vector<HashEntry> array_;
  int current_size_;
	
  // private memeber variable that counts the number of collisions in the hash table
  int collisions_counter_;

  bool IsActive(int current_pos) const
  { return array_[ current_pos ].info_ == kActive; }
  int FindPos(const HashedObj & x, int & probes) const;
  void Rehash(); 

  size_t InternalHash(const HashedObj & x) const {
    static hash<HashedObj> hf;
    return hf(x) % array_.size();
  }
	
  // second hash function: R - (x % R); R = 43
  size_t SecondHash(const HashedObj & x) const {
    static hash<HashedObj> hf;
    return (43-(hf(x) % 43));
  }
};

#include "DoubleHashing.cpp"  // For template compilation.
#endif  // CSCI335_HASHING_DOUBLE_HASHING_H_
