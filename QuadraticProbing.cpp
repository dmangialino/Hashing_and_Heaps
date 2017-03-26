// Dina Mangialino
// CSCI 335 Assignment 3 - Hashing and Heaps
// March 26, 2016

// QuadraticProbing.cpp
// Provided code that implements Quadratic Probing.

template <typename HashedObj>
bool HashTable<HashedObj>::Contains(const HashedObj & x, int & probes) const {
  // set probes to 1 here to count as first access is FindPos
  // cannot count this access in FindPos because the same variable is used to count the number of Collisions in Insert, 
  // so including it there would count it as a collision
  probes = 1;
  int temp = FindPos(x, probes);
  return IsActive(temp);
}

template <typename HashedObj>
void HashTable<HashedObj>::MakeEmpty() {
  current_size_ = 0;
    collisions_counter_ = 0;
  for (auto & entry : array_)
    entry.info_ = kEmpty;
}

template <typename HashedObj>
bool HashTable<HashedObj>::Insert(const HashedObj & x) {
  int probes = 0;
	
  // Insert x as active.
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
bool HashTable<HashedObj>::Insert(HashedObj && x) {
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
bool HashTable<HashedObj>::Remove(const HashedObj & x) {
  int current_pos = FindPos(x);
  if (!IsActive(current_pos))
    return false;
  
  array_[current_pos].info_ = kDeleted;
  return true;
}

template <typename HashedObj>
int HashTable<HashedObj>::FindPos(const HashedObj & x, int & probes) const {
  int offset = 1;
  int current_pos = InternalHash(x);
  
  while (array_[current_pos].info_ != kEmpty && array_[current_pos].element_ != x ) {
    ++probes;
    current_pos += offset;  
    offset += 2;
    if (current_pos >= array_.size())
      current_pos -= array_.size( );
  }

  return current_pos;
}

template <typename HashedObj>
void HashTable<HashedObj>::Rehash() {
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
int HashTable<HashedObj>::CurrentSize() {
  return current_size_;
}

// reutrns size of the hash table
template <typename HashedObj>
int HashTable<HashedObj>::TableSize() {
  return array_.size();
}

// returns the number of collisions
template <typename HashedObj>
int HashTable<HashedObj>::CollisionCount() {
  return collisions_counter_;
}






