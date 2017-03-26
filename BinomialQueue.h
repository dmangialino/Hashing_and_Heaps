// Dina Mangialino
// CSCI 335 Assignment 3 - Hashing and Heaps
// April 2, 2016

// BinomialQueue.h
// Provided class that implements a binomial queue.

// DeleteMin changed to print the minimum element
// NewInsert methods added to insert an element without using Merge for Part B
// Insert3, DeleteMin3, and Merge3 are for Part C of the assignment

#ifndef COURSES_CSCI335_BINOMIAL_QUEUE_H_
#define COURSES_CSCI335_BINOMIAL_QUEUE_H_

#include <iostream>
#include <vector>
#include "dsexceptions.h"
using namespace std;

// Binomial queue class
//
// CONSTRUCTION: with no parameters
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// deleteMin( )           --> Return and remove smallest item
// Comparable findMin( )  --> Return smallest item
// bool isEmpty( )        --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void merge( rhs )      --> Absorb rhs into this heap
// ******************ERRORS********************************
// Throws UnderflowException as warranted

template <typename Comparable>
class BinomialQueue {
 public:
  
  BinomialQueue(): the_trees_(1) {
    for (auto & root : the_trees_)
      root = nullptr;
    current_size_ = 0;
  }
 
  BinomialQueue(const BinomialQueue &rhs): the_trees_(rhs.the_trees_.size()),
    current_size_{rhs.current_size_} { 
    for (int i = 0; i < rhs.the_trees_.size(); ++i)
      the_trees_[i] = clone(rhs.the_trees_[i]);
  }
  
  BinomialQueue(BinomialQueue &&rhs): 
  the_trees_{std::move(rhs.the_trees_)}, current_size_{rhs.current_size_} { }
  
  BinomialQueue(const Comparable &item) : the_trees_(1), current_size_{1} {
    the_trees_[0] = new BinomialNode{item, nullptr, nullptr}; }
  
  BinomialQueue & operator=(const BinomialQueue &rhs) {
    BinomialQueue copy = rhs;
    std::swap(*this, copy);
    return *this;
  }
        
  BinomialQueue & operator=(BinomialQueue && rhs) {
    std::swap(current_size_, rhs.current_size_);
    std::swap(the_trees_, rhs.the_trees_);
    
    return *this;
  }
 
  ~BinomialQueue() { MakeEmpty(); }

  bool IsEmpty() const {return current_size_ == 0;}
  
  const Comparable &FindMin( ) const {
    if (IsEmpty())
      throw UnderflowException{ };
    
    return the_trees_[FindMinIndex()]->element_;
  }
  
  void Insert(const Comparable & x) {
    BinomialQueue one_item_queue{x}; 
    Merge(one_item_queue); 
  }
	
  void Insert(Comparable && x) { 
    BinomialQueue one_item_queue{std::move(x)}; 
    Merge(one_item_queue); 
  }
	
  // INSERT METHODS FOR PART B
	
  // Insert item x into the priority queue without calling Merge()
  void NewInsert(const Comparable & x) {
    BinomialQueue one_item_queue{x};
		
    current_size_ += one_item_queue.current_size_;
		
    if (current_size_ > Capacity()) {
      int old_number_of_trees = the_trees_.size();
      int new_number_of_trees = max(the_trees_.size(), one_item_queue.the_trees_.size()) + 1;
      the_trees_.resize(new_number_of_trees);
      for (int i = old_number_of_trees; i < new_number_of_trees; ++i)
        the_trees_[i] = nullptr;
    }
		
    BinomialNode *carry = nullptr;
		
    BinomialNode *t1 = the_trees_[0];
    BinomialNode *t2 = 0 < one_item_queue.the_trees_.size() ? one_item_queue.the_trees_[ 0 ] : nullptr;
		
    // if the current size of the queue is odd, that means there are an even number of elements in this tree + 1 element in rhs (the item to be inserted)
    // in this case, the new element is simply added as the tree of 1
    if (current_size_ % 2 != 0) {
      the_trees_[0] = t2;
      one_item_queue.the_trees_[0] = nullptr;
    } else {
      // total size of the queue is even, so this had an odd number of elements (tree of just one element was occupied)
			
      // combine the two trees and make the tree of one element and the one_item_queue (the new queue) point to null
      carry = CombineTrees(t1, t2);
      the_trees_[0] = one_item_queue.the_trees_[0] = nullptr;	
			
      // continue to combine the tree and carry 
      int i, j;
      for (i = 1, j = 2; j <= current_size_; ++i, j *= 2) {
        t1 = the_trees_[i];
        t2 = i < one_item_queue.the_trees_.size() ? one_item_queue.the_trees_[ i ] : nullptr;
			
        int which_case = t1 == nullptr ? 0 : 1;
        which_case += t2 == nullptr ? 0 : 2;
        which_case += carry == nullptr ? 0 : 4;
      
        switch (which_case) {
          // only carry tree
          case 4:  
            the_trees_[i] = carry;
            carry = nullptr;
            break;
          // this and carry trees
          case 5:  
            carry = CombineTrees(t1, carry);
            the_trees_[i] = nullptr;
            break;
        }
      }
    }
		
    // Invalidate one_item_queue
    for (auto & root : one_item_queue.the_trees_)
      root = nullptr;
    one_item_queue.current_size_ = 0;	
  }
	
  void NewInsert(const Comparable && x) {
    BinomialQueue one_item_queue{x};
		
    current_size_ += one_item_queue.current_size_;
		
    if (current_size_ > Capacity()) {
      int old_number_of_trees = the_trees_.size();
      int new_number_of_trees = max(the_trees_.size(), one_item_queue.the_trees_.size()) + 1;
      the_trees_.resize(new_number_of_trees);
      for (int i = old_number_of_trees; i < new_number_of_trees; ++i)
        the_trees_[i] = nullptr;
    }
		
    BinomialNode *carry = nullptr;
		
    BinomialNode *t1 = the_trees_[0];
    BinomialNode *t2 = 0 < one_item_queue.the_trees_.size() ? one_item_queue.the_trees_[ 0 ] : nullptr;
		
    if (current_size_ % 2 != 0) {
      the_trees_[0] = t2;
      one_item_queue.the_trees_[0] = nullptr;
    } else {
      carry = CombineTrees(t1, t2);
      the_trees_[0] = one_item_queue.the_trees_[0] = nullptr;	
			
      int i, j;
      for (i = 1, j = 2; j <= current_size_; ++i, j *= 2) {
        t1 = the_trees_[i];
        t2 = i < one_item_queue.the_trees_.size() ? one_item_queue.the_trees_[ i ] : nullptr;
			
        int which_case = t1 == nullptr ? 0 : 1;
        which_case += t2 == nullptr ? 0 : 2;
        which_case += carry == nullptr ? 0 : 4;
      
        switch (which_case) {
          // only carry tree
          case 4:
            the_trees_[i] = carry;
            carry = nullptr;
					  break;
					// this and carry trees
					case 5:  
            carry = CombineTrees(t1, carry);
            the_trees_[i] = nullptr;
            break;
        }
      }
    }
		
    // invalidate one_item_queue tree.
    for (auto & root : one_item_queue.the_trees_)
      root = nullptr;
    one_item_queue.current_size_ = 0;	
  }
	
  // INSERT METHODS FOR PART C
	
  // Insert item x into the priority queue using Merge3(); allows duplicates.
  void Insert3(const Comparable & x) {
    BinomialQueue one_item_queue{x}; 
    Merge3(one_item_queue); 
  }
	
  void Insert3(const Comparable && x) {
    BinomialQueue one_item_queue{x}; 
    Merge3(one_item_queue); 
  }
    
  void DeleteMin() {
    Comparable x;
    DeleteMin(x);
  }

  void DeleteMin(Comparable & min_item) {
    if (IsEmpty())
      throw UnderflowException{};

    const int min_index = FindMinIndex( );
    min_item = the_trees_[min_index]->element_;
		
    // prints the minimum item that will be deleted from the queue
    cout << min_item << ' ';

    BinomialNode *old_root = the_trees_[min_index];
    BinomialNode *deleted_tree = old_root->left_child_;
    delete old_root;
    
    BinomialQueue deleted_queue;
    deleted_queue.the_trees_.resize(min_index + 1);
    deleted_queue.current_size_ = (1 << min_index) - 1;
    for (int j = min_index - 1; j >= 0; --j) {
      deleted_queue.the_trees_[j] = deleted_tree;
      deleted_tree = deleted_tree->next_sibling_;
      deleted_queue.the_trees_[j]->next_sibling_ = nullptr;
    }

    the_trees_[min_index] = nullptr;
    current_size_ -= deleted_queue.current_size_ + 1;

    Merge(deleted_queue);
  }

  // Remove the smallest item from the priority queue.
  // Throws UnderflowException if empty.
  // Uses Merge3() created for part C
  void DeleteMin3() {
    Comparable x;
    DeleteMin3(x);
  }

  // Remove the minimum item and place it in min_item.
  // Throws UnderflowException if empty.
  // Uses Merge3() created for part C
  void DeleteMin3(Comparable & min_item) {
    if (IsEmpty())
      throw UnderflowException{};

    const int min_index = FindMinIndex( );
    min_item = the_trees_[min_index]->element_;
		
    cout << min_item << ' ';

    BinomialNode *old_root = the_trees_[min_index];
    BinomialNode *deleted_tree = old_root->left_child_;
    delete old_root;
		
    BinomialQueue deleted_queue;
    deleted_queue.the_trees_.resize(min_index + 1);
    deleted_queue.current_size_ = (1 << min_index) - 1;
    for (int j = min_index - 1; j >= 0; --j) {
      deleted_queue.the_trees_[j] = deleted_tree;
      deleted_tree = deleted_tree->next_sibling_;
      deleted_queue.the_trees_[j]->next_sibling_ = nullptr;
    }

    the_trees_[min_index] = nullptr;
    current_size_ -= deleted_queue.current_size_ + 1;

    Merge3(deleted_queue);
  }

  void MakeEmpty() {
    current_size_ = 0;
    for (auto & root : the_trees_)
      MakeEmpty(root);
  }

  void Merge(BinomialQueue & rhs) {
    if (this == &rhs)
      return;

    current_size_ += rhs.current_size_;

    if (current_size_ > Capacity()) {
      int old_number_of_trees = the_trees_.size();
      int new_number_of_trees = max(the_trees_.size(), rhs.the_trees_.size()) + 1;
      the_trees_.resize(new_number_of_trees);
      for (int i = old_number_of_trees; i < new_number_of_trees; ++i)
        the_trees_[i] = nullptr;
    }

    BinomialNode *carry = nullptr;

    for (int i = 0, j = 1; j <= current_size_; ++i, j *= 2) {
      BinomialNode *t1 = the_trees_[i];
      BinomialNode *t2 = i < rhs.the_trees_.size() ? rhs.the_trees_[ i ] : nullptr;
      
      int which_case = t1 == nullptr ? 0 : 1;
      which_case += t2 == nullptr ? 0 : 2;
      which_case += carry == nullptr ? 0 : 4;
      
      switch (which_case) {
        // only this tree 
        case 0:
        // only this tree
        case 1:
          break;
        // only rhs tree
        case 2:  
          the_trees_[i] = t2;
          rhs.the_trees_[i] = nullptr;
          break;
        // only carry tree
        case 4: 
          the_trees_[i] = carry;
          carry = nullptr;
          break;
        // this and rhs trees
        case 3:  
          carry = CombineTrees(t1, t2);
          the_trees_[i] = rhs.the_trees_[i] = nullptr;
          break;
        // this and carry trees
        case 5:  
          carry = CombineTrees(t1, carry);
          the_trees_[i] = nullptr;
          break;
        // rhs and carry trees
        case 6:  
          carry = CombineTrees(t2, carry);
          rhs.the_trees_[i] = nullptr;
          break;
        // this, rhs and carry trees
        case 7: 
          the_trees_[i] = carry;
          carry = CombineTrees(t1, t2);
          rhs.the_trees_[i] = nullptr;
          break;
      }
    }
    
    for (auto & root : rhs.the_trees_)
      root = nullptr;
    rhs.current_size_ = 0;
  }  

  void Merge3(BinomialQueue & rhs) {
    if (this == &rhs) 
      return;
    
    // if the rhs tree is larger, swap the trees so the smaller tree always merges under the larger one
    if (current_size_ < rhs.current_size_) {
      swap(current_size_, rhs.current_size_);
      swap(the_trees_, rhs.the_trees_);
    } 
		
    current_size_ += rhs.current_size_;

    if (current_size_ > Capacity()) {
      int old_number_of_trees = the_trees_.size();
      int new_number_of_trees = max(the_trees_.size(), rhs.the_trees_.size()) + 1;
      the_trees_.resize(new_number_of_trees);
      for (int i = old_number_of_trees; i < new_number_of_trees; ++i)
        the_trees_[i] = nullptr;
    }

    BinomialNode *carry = nullptr;

    for (int i = 0, j = 1; j <= current_size_; ++i, j *= 2) {
      BinomialNode *t1 = the_trees_[i];
      BinomialNode *t2 = i < rhs.the_trees_.size() ? rhs.the_trees_[ i ] : nullptr;
      			
      int which_case = t1 == nullptr ? 0 : 1;
      which_case += t2 == nullptr ? 0 : 2;
      which_case += carry == nullptr ? 0 : 4;
			
      switch (which_case) {
        // No trees
        case 0:  
          break;
        // Only this tree
        case 1: 
          // if there are no more trees in rhs and no carry, invalidate rhs tree and terminate merging (return from the function)
          if (i == rhs.the_trees_.size()) {
            for (auto & root : rhs.the_trees_)
              root = nullptr;
          rhs.current_size_ = 0;
          return; 
				  } else {
				    break;
				  }
        // Only rhs tree
        case 2:  
          the_trees_[i] = t2;
          rhs.the_trees_[i] = nullptr;
          break;
        // Only carry tree
        case 4:  
          the_trees_[i] = carry;
          carry = nullptr;
          break;
        // this and rhs trees
        case 3:  
          carry = CombineTrees(t1, t2);
          the_trees_[i] = rhs.the_trees_[i] = nullptr;
          break;
        // this and carry trees
        case 5:  
          carry = CombineTrees(t1, carry);
          the_trees_[i] = nullptr;
          break;
        // rhs and carry trees
        case 6: 
          carry = CombineTrees(t2, carry);
          rhs.the_trees_[i] = nullptr;
          break;
        // this, rhs and carry trees
        case 7: 
          the_trees_[i] = carry;
          carry = CombineTrees(t1, t2);
          rhs.the_trees_[i] = nullptr;
          break;
      }
    }
    
    // Invalidate rhs tree
    for (auto & root : rhs.the_trees_)
      root = nullptr;
    rhs.current_size_ = 0;
  }
	
  int CurrentSize() {
    return current_size_;
  }

  private:
  struct BinomialNode {
    Comparable    element_;
    BinomialNode *left_child_;
    BinomialNode *next_sibling_;
    
    BinomialNode(const Comparable & e, BinomialNode *lt, BinomialNode *rt)
     : element_{e}, left_child_{lt}, next_sibling_{rt} {}
    
    BinomialNode(Comparable &&e, BinomialNode *lt, BinomialNode *rt)
     : element_{ std::move(e) }, left_child_{lt}, next_sibling_{rt} {}
  };

  vector<BinomialNode *> the_trees_;  
  int current_size_;                  
	
  int FindMinIndex() const {
    int i;
    int min_index;
    
    for (i = 0; the_trees_[i] == nullptr; ++i);

    for (min_index = i; i < the_trees_.size(); ++i)
      if (the_trees_[i] != nullptr && the_trees_[i]->element_ < the_trees_[min_index]->element_)
        min_index = i;
    
    return min_index;
  }

  int Capacity() const { 
    return (1 << the_trees_.size()) - 1; 
  }
  
  BinomialNode * CombineTrees(BinomialNode *t1, BinomialNode *t2) {
    if (t2->element_ < t1->element_) 
      return CombineTrees( t2, t1 );

    t2->next_sibling_ = t1->left_child_;
    t1->left_child_ = t2;
    return t1;
  }

  void MakeEmpty(BinomialNode * & t) {
    if (t != nullptr) {
      MakeEmpty(t->left_child_);
      MakeEmpty(t->next_sibling_);
      delete t;
      t = nullptr;
    }
  }
  
  BinomialNode * clone(BinomialNode * t) const {
    if (t == nullptr)
      return nullptr;
    else
      return new BinomialNode{ t->element_, clone(t->left_child_), clone(t->next_sibling_) };
  }
};

#endif  // COURSES_CSCI335_BINOMIAL_QUEUE_H_
