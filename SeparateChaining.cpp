// Dina Mangialino
// CSCI 335 Assignment 3 - Hashing and Heaps
// March 26, 2016

// SeparateChainging.cpp
// Provided code that implements Separate Chaining.

template <typename HashedObj>
bool SepChainHashTable<HashedObj>::Contains(const HashedObj &x, int & probes) const {
  probes = 0;
  auto &which_list = the_lists_[InternalHash(x)];
  ++probes;
  for (auto y : which_list) {
    if (y == x)
      return true;
    else
      ++probes;
  }
	
  return false;
}
  
template <typename HashedObj>
void SepChainHashTable<HashedObj>::MakeEmpty() {
  for (auto &this_list : the_lists_)
    this_list.clear();
}
  
template <typename HashedObj>
bool SepChainHashTable<HashedObj>::Insert(const HashedObj & x) {
  auto &which_list = the_lists_[InternalHash(x)];
  for (auto y : which_list) {
    if (y == x) 
      return false;
    else 
      ++collisions_counter_;
  }
  which_list.push_back(x);
  if (++current_size_ > the_lists_.size())
    Rehash();
  return true;
}
    
template <typename HashedObj>
bool SepChainHashTable<HashedObj>::Insert(HashedObj && x) {
  auto &which_list = the_lists_[InternalHash(x)];
  for (auto y : which_list) {
    if (y == x) 
      return false;
    else 
      ++collisions_counter_;
  }
  which_list.push_back(std::move(x));
  if (++current_size_ > the_lists_.size())
    Rehash();
  return true;
}

template <typename HashedObj>
bool SepChainHashTable<HashedObj>::Remove(const HashedObj & x) {
  auto &which_list = the_lists_[InternalHash(x)];
  auto itr = find(begin(which_list), end(which_list), x);
  if (itr == end(which_list))
    return false;
  which_list.erase(itr);
  --current_size_;
  return true;
}

template <typename HashedObj>
void SepChainHashTable<HashedObj>::Rehash() {
  vector<list<HashedObj>> old_lists = the_lists_;
	
  // save the current number of collisions to restore at the end of the Rehash() 
  // so collisions during reshashing are not counted as collisions in the hash table
  int temp_col_counter_ = collisions_counter_;
	
  the_lists_.resize(NextPrime(2 * the_lists_.size()));
  for (auto & this_list: the_lists_)
    this_list.clear();
  
  current_size_ = 0;
  for (auto & this_list : old_lists) {
    for (auto & x : this_list )
      Insert(std::move(x));
  }
	
  collisions_counter_ = temp_col_counter_;
}


// NEW METHODS

// returns current size of the hash table
template <typename HashedObj>
int SepChainHashTable<HashedObj>::CurrentSize() {
  return current_size_;
}

// returns size of the table
template <typename HashedObj>
int SepChainHashTable<HashedObj>::TableSize() {
  return the_lists_.size();
}

// returns the number of collisions in the table
template <typename HashedObj>
int SepChainHashTable<HashedObj>::CollisionCount() {
  return collisions_counter_;
}














