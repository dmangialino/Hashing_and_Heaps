// Dina Mangialino
// CSCI 335 Assignment 3 - Hashing and Heaps
// March 26, 2016

// LinearProbing.cpp
// Implements Linear Probing.

template <typename HashedObj>
bool LinearProbHashTable<HashedObj>::Contains(const HashedObj & x, int & probes) const {
  // set probes to 1 here to count as first access is FindPos
  // cannot count this access in FindPos because the same variable is used to count the number of Collisions in Insert,
  // so including it there would count an additional collision
  probes = 1;
  int temp = FindPos(x, probes);
  return IsActive(temp);
}

template <typename HashedObj>
void LinearProbHashTable<HashedObj>::MakeEmpty() {
  current_size_ = 0;
  collisions_counter_ = 0;
  for (auto & entry : array_)
    entry.info_ = kEmpty;
}

template <typename HashedObj>
bool LinearProbHashTable<HashedObj>::Insert(const HashedObj & x) {
  int probes = 0;
	
  int current_pos = FindPos(x, probes);
  if (IsActive(current_pos))
    return false;
  
  array_[current_pos].element_ = x;
  array_[current_pos].info_ = kActive;
  
  if (++current_size_ > array_.size() / 2)
    Rehash();
  
  collisions_counter_ += probes;
  return true;
}
    
template <typename HashedObj>
bool LinearProbHashTable<HashedObj>::Insert(HashedObj && x) {
  int probes = 0;
	
  int current_pos = FindPos(x, probes);
  if (IsActive(current_pos))
    return false;
  
  array_[current_pos] = std::move(x);
  array_[current_pos].info_ = kActive;
  
  if (++current_size_ > array_.size() / 2)
    Rehash();
		
  collisions_counter_ += probes;
  return true;
}

template <typename HashedObj>
bool LinearProbHashTable<HashedObj>::Remove(const HashedObj & x) {
  int current_pos = FindPos(x);
  if (!IsActive(current_pos))
    return false;
  
  array_[current_pos].info_ = kDeleted;
  return true;
}

template <typename HashedObj>
int LinearProbHashTable<HashedObj>::FindPos(const HashedObj & x, int &probes) const {
  int current_pos = InternalHash(x);
  
  while (array_[current_pos].info_ != kEmpty && array_[current_pos].element_ != x ) {
    ++probes;
    // next probe is the next position in the table (current_pos + 1)
    ++current_pos;
    if (current_pos >= array_.size())
      current_pos -= array_.size( );
  }
  return current_pos;
}

template <typename HashedObj>
void LinearProbHashTable<HashedObj>::Rehash() {
  vector<HashEntry> old_array = array_;
	
  // save the current number of collisions to restore at the end of the Rehash() 
  // so collisions during reshashing are not counted as collisions in the hash table
  int temp_col_counter_ = collisions_counter_;
  array_.resize(NextPrime(2 * old_array.size()));
  for (auto & entry : array_)
    entry.info_ = kEmpty;
  
  current_size_ = 0;
  for (auto & entry : old_array)
    if (entry.info_ == kActive)
      Insert(std::move(entry.element_));
	
  collisions_counter_ = temp_col_counter_;
}

// returns current size of the hash table
template <typename HashedObj>
int LinearProbHashTable<HashedObj>::CurrentSize() {
  return current_size_;
}

// reutrns size of the hash table
template <typename HashedObj>
int LinearProbHashTable<HashedObj>::TableSize() {
  return array_.size();
}

// returns the number of collisions
template <typename HashedObj>
int LinearProbHashTable<HashedObj>::CollisionCount() {
  return collisions_counter_;
}



