// Dina Mangialino
// CSCI 335 Assignment 3 - Hashing and Heaps
// March 26, 2016

// QuadraticProbing.h
// Provided header file for class that implements hash table with quadratic probing.

// Collision counter and probe counters added
// CurrentSize(), TableSize(), and CollisionCount() public methods added

#ifndef CSCI335_HASHING_QUADRATIC_PROBING_H_
#define CSCI335_HASHING_QUADRATIC_PROBING_H_

#include <iostream>
#include <vector>
using namespace std;

namespace {
  bool IsPrime(int n) {
    if (n == 2 || n == 3)
      return true;
    if (n == 1 || n % 2 == 0)
      return false;
    for (int i = 3; i * i <= n; i += 2)
      if (n % i == 0)
        return false;
    return true;
  }

  int NextPrime(int n) {
    if (n % 2 == 0) ++n;
    for ( ;!IsPrime(n); n += 2);
    return n;
  }
}  

template <typename HashedObj>
class HashTable {
 public:
  enum EntryType {kActive, kEmpty, kDeleted};

  explicit HashTable(int size = 101) : array_(NextPrime(size))
    { MakeEmpty(); }
  HashTable(const HashTable &a_hash_table) = default;
  HashTable(HashTable &&a_hash_table) = default;
  HashTable &operator=(const HashTable &a_hash_table) = default;
  HashTable &operator=(HashTable &&a_hash_table) = default;
  ~HashTable() = default;

  bool Contains(const HashedObj & x, int & probes) const;
  void MakeEmpty();
  bool Insert(const HashedObj & x);\
  bool Insert(HashedObj && x);
  bool Remove(const HashedObj & x);
	
  // NEW METHODS
  // returns current size of the hash table
  int CurrentSize();
  // reutrns size of the hash table
  int TableSize();
  // returns number of collisions
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
  int FindPos(const HashedObj & x, int &probes) const;
  void Rehash(); 
  size_t InternalHash(const HashedObj & x) const {
    static hash<HashedObj> hf;
    return hf(x) % array_.size();
  }
};

#include "QuadraticProbing.cpp"  // For template compilation.
#endif  // CSCI335_HASHING_QUADRATIC_PROBING_H_



