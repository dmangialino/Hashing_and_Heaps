// Dina Mangialino
// CSCI 335 Assignment 3 - Hashing and Heaps
// March 26, 2016

// SeparateChainging.h
// Provided class that implements hash table that uses Separate Chaining.

// collision counter and probe counters added
// CurrentSize(), TableSize(), and CollisionCount() public methods added

#ifndef CSCI335_HASHING_SEPARATE_CHAINING_H_
#define CSCI335_HASHING_SEPARATE_CHAINING_H_

#include <vector>
#include <list>
using namespace std;

template <typename HashedObj>
class SepChainHashTable {
 public:
  explicit SepChainHashTable(int size = 101) : current_size_{0} 
  { the_lists_.resize(101); }

  SepChainHashTable(const SepChainHashTable &a_hash_table) = default;
  SepChainHashTable(SepChainHashTable &&a_hash_table) = default;
  SepChainHashTable &operator=(const SepChainHashTable &a_hash_table) = default;
  SepChainHashTable &operator=(SepChainHashTable &&a_hash_table) = default;
  ~SepChainHashTable() = default;
  
  bool Contains(const HashedObj &x, int &probes) const;
  void MakeEmpty();
  bool Insert(const HashedObj & x);
  bool Insert(HashedObj && x);
  bool Remove(const HashedObj & x);
	
  // new methods
  int CurrentSize();
  int TableSize();
  int CollisionCount();

 private:
  vector<list<HashedObj>> the_lists_;
  int current_size_;
	
  // private memeber variable that counts the number of collisions in the hash table
  int collisions_counter_;

  void Rehash();
	
  size_t InternalHash(const HashedObj & x) const {
    static hash<HashedObj> hf;
    return hf(x) % the_lists_.size( );
  }
};

#include "SeparateChaining.cpp"  // For template compilation.
#endif  // CSCI335_HASHING_SEPARATE_CHAINING_H_
