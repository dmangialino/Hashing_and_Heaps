// Dina Mangialino
// CSCI 335 Assignment 3 - Hashing and Heaps
// March 31, 2016

// DoubleHashing.cpp
// Implements Double Hashing.

template <typename HashedObj>
bool DoubleHashTable<HashedObj>::Contains(const HashedObj & x, int & probes) const {
  // set probes to 1 here to count as first access is FindPos
  // cannot count this access in FindPos because the same variable is used to count the number of Collisions in Insert, 
  // so including it there would count it as a collision
  probes = 1;
  int temp = FindPos(x, probes);
  return IsActive(temp);
}

template <typename HashedObj>
void DoubleHashTable<HashedObj>::MakeEmpty() {
  current_size_ = 0;
  collisions_counter_ = 0;
  for (auto & entry : array_)
    entry.info_ = kEmpty;
}

template <typename HashedObj>
bool DoubleHashTable<HashedObj>::Insert(const HashedObj & x) {
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
bool DoubleHashTable<HashedObj>::Insert(HashedObj && x) {
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
bool DoubleHashTable<HashedObj>::Remove(const HashedObj & x) {
  int current_pos = FindPos(x);
  if (!IsActive(current_pos))
    return false;
  
  array_[current_pos].info_ = kDeleted;
  return true;
}

template <typename HashedObj>
int DoubleHashTable<HashedObj>::FindPos(const HashedObj & x, int & probes) const {
  int offset = 1;
  int current_pos = InternalHash(x);
  int org_hash = current_pos;
  // save value of the second hash function so it only has the be calculated once
  int second_hash_result = SecondHash(x);
  
  while (array_[current_pos].info_ != kEmpty && array_[current_pos].element_ != x ) {
    ++probes;
    // next position = offset * value of the second hash function + original hash
    // offset = i where i is the i-th probe
    current_pos = offset * second_hash_result + org_hash;  
    ++offset;
    while (current_pos >= array_.size())
      current_pos -= array_.size();
  }
	
  return current_pos;
}

template <typename HashedObj>
void DoubleHashTable<HashedObj>::Rehash() {
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

// NEW METHODS

// returns current size of the hash table
template <typename HashedObj>
int DoubleHashTable<HashedObj>::CurrentSize() {
  return current_size_;
}

// reutrns size of the hash table
template <typename HashedObj>
int DoubleHashTable<HashedObj>::TableSize() {
  return array_.size();
}

// returns the number of collisions
template <typename HashedObj>
int DoubleHashTable<HashedObj>::CollisionCount() {
  return collisions_counter_;
}






