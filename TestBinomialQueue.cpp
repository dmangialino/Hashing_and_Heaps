// Dina Mangialino
// CSCI 335 Assignment 3 - Hashing and Heaps
// April 2, 2016

// TestBinomialQueue.cpp
// Part 3: Heaps

// If the flag is 0, all the integers will be read directly into one input queue
// Program will call:
// FlagZero: Part A, uses provided methods
// FlagZero2: Part B, uses new insert function that does not call merge
// FlagZero3: Part C, uses modified merge that terminates if there are no trees in H2 and carry pointer is null; 
//   always merges smaller tree under the larger tree
// All three functions add the integers to a queue and report the time this process takes. 
// They then call DeleteMin() on the queues until they are empty.

// If the flag is not zero, the program will call
// FlagNonZero: Part A
// FlagNonZero2: Part B
// FlagNonZero3: Part C
// In these functions, the first quarter of the integers will be read into one queue and the rest into a second queue. 
// The two will then be merged and the time this process took will be reported. 
// Then the first ten items, or, if the queue contains fewer than ten items, the number of items in the queue, 
//   will be deleted by calling DeleteMin() on the queue ten times (or up to ten times if the queue has fewer than ten elements).

#include "BinomialQueue.h"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>
#include <vector>
using namespace std;

// Part C for zero flag (add all numbers directly into one input queue)
// uses modified merge that terminates if there are no trees in H2 and carry tree pointer is null
// always merges smaller tree under larger tree
void FlagZero3(string & input_filename) {
  cout << endl << "PART C: MODIFIED MERGE" << endl;
	
  const auto begin = chrono::high_resolution_clock::now();

  int sum = 0;
  for (int i = 1; i < 10000; ++i) sum ++;
	
  ifstream fin;
  fin.open(input_filename);

  BinomialQueue<int> input_queue;
	
  int num;
  while (fin >> num) {
    input_queue.Insert3(num);
  }
	
  const auto end = chrono::high_resolution_clock::now();
  cout << chrono::duration_cast<chrono::nanoseconds>(end-begin).count() << "ns" << endl;
  cout << chrono::duration_cast<chrono::milliseconds>(end-begin).count() << "ms" << endl;
	
  fin.close();
	
  // CurrentSize() is a new public method that returns the size of the queue
  // DeleteMin() was updated to print the minimum element before deleting it
  while (input_queue.CurrentSize() > 0)
    input_queue.DeleteMin3();
  cout << endl;
}

// Part B for zero flag (add all numbers directly into one input queue)
// uses new insert that does not call a merge function
void FlagZero2(string & input_filename) {
  cout << endl << "PART B: INSERT WITHOUT MERGE" << endl;
	
  const auto begin = chrono::high_resolution_clock::now();

  int sum = 0;
  for (int i = 1; i < 10000; ++i) sum ++;
	
  ifstream fin;
  fin.open(input_filename);
	
  BinomialQueue<int> input_queue;
	
  int num;
  while (fin >> num)
    input_queue.NewInsert(num);
	
  const auto end = chrono::high_resolution_clock::now();
  cout << chrono::duration_cast<chrono::nanoseconds>(end-begin).count() << "ns" << endl;
  cout << chrono::duration_cast<chrono::milliseconds>(end-begin).count() << "ms" << endl;
	
  fin.close();
	
  // CurrentSize() is a new public method that returns the size of the queue
  // DeleteMin() was updated to print the minimum element before deleting it
  while (input_queue.CurrentSize() > 0)
    input_queue.DeleteMin();
  cout << endl;
	
  FlagZero3(input_filename);
}

// Part A for zero flag (add all numbers direclty into one input queue)
// uses provided methods
void FlagZero(string & input_filename) {
  cout << "PART A: INSERT WITH PROVIDED METHODS" << endl;
	
  const auto begin = chrono::high_resolution_clock::now();

  int sum = 0;
  for (int i = 1; i < 10000; ++i) sum ++;
	
  ifstream fin;
  fin.open(input_filename);
	
  BinomialQueue<int> input_queue;
	
  int num;
  while (fin >> num)
    input_queue.Insert(num);
	
  const auto end = chrono::high_resolution_clock::now();
  cout << chrono::duration_cast<chrono::nanoseconds>(end-begin).count() << "ns" << endl;
  cout << chrono::duration_cast<chrono::milliseconds>(end-begin).count() << "ms" << endl;
	
  fin.close();
	
  // CurrentSize() is a new public method that returns the size of the queue
  // DeleteMin() was updated to print the minimum element before deleting it
  while (input_queue.CurrentSize() > 0)
    input_queue.DeleteMin();
  cout << endl;
	
  FlagZero2(input_filename);
}

// Part C for nonzero flag (add numbers into two queues, then merge them)
// uses modified merge that terminates if there are no trees in H2 and carry tree pointer is null
// always merges smaller tree under larger tree
void FlagNonZero3(string & input_filename) {
  cout << endl << "PART C: MODIFIED MERGE" << endl;
	
  char split_char = '_';
  vector<string> temp;
  istringstream split(input_filename);
  for (string str; getline(split, str, split_char); temp.push_back(str));
  int size = stoi(temp[0]);
  int quarter = size/4;
	
  const auto begin = chrono::high_resolution_clock::now();

  int sum = 0;
  for (int i = 1; i < 10000; ++i) sum ++;	

  ifstream fin;
  fin.open(input_filename);

  BinomialQueue<int> bq1;
  int num;
  for (int i = 0; i < quarter; ++i) {
    fin >> num;
    bq1.Insert3(num);
  }
	
  BinomialQueue<int> bq2;
  while (fin >> num)
    bq2.Insert3(num);
	
  bq2.Merge3(bq1);
	
  const auto end = chrono::high_resolution_clock::now();
  cout << chrono::duration_cast<chrono::nanoseconds>(end-begin).count() << "ns" << endl;
  cout << chrono::duration_cast<chrono::milliseconds>(end-begin).count() << "ms" << endl;

  fin.close();
  
  // if check to avoid an error of the queue contains fewer than 10 elements
  if (bq2.CurrentSize() >= 10) {
    for (int i = 0; i < 10; ++i)
      bq2.DeleteMin3();
    cout << endl;
  } else {
    while (bq2.CurrentSize() > 0)
      bq2.DeleteMin3();
    cout << endl;
  }
}

// Part B for nonzero flag (add numbers into two queues, then merge them)
// uses new insert that does not call a merge function
void FlagNonZero2(string & input_filename) {
  cout << endl << "PART B: INSERT WITHOUT MERGE" << endl;
	
  char split_char = '_';
  vector<string> temp;
  istringstream split(input_filename);
  for (string str; getline(split, str, split_char); temp.push_back(str));
  int size = stoi(temp[0]);
  int quarter = size/4;

  const auto begin = chrono::high_resolution_clock::now();

  int sum = 0;
  for (int i = 1; i < 10000; ++i) sum ++;	
	
  ifstream fin;
  fin.open(input_filename);
	
  BinomialQueue<int> bq1;
  int num;
  for (int i = 0; i < quarter; ++i) {
    fin >> num;
    bq1.NewInsert(num);
  }
	
  BinomialQueue<int> bq2;
  while (fin >> num)
    bq2.NewInsert(num);
	
  bq2.Merge(bq1);
	
  const auto end = chrono::high_resolution_clock::now();
  cout << chrono::duration_cast<chrono::nanoseconds>(end-begin).count() << "ns" << endl;
  cout << chrono::duration_cast<chrono::milliseconds>(end-begin).count() << "ms" << endl;

  fin.close();
  
  // if check to avoid an error of the queue contains fewer than 10 elements
  if (bq2.CurrentSize() >= 10) {
    for (int i = 0; i < 10; ++i)
      bq2.DeleteMin();
    cout << endl;
  } else {
    while (bq2.CurrentSize() > 0)
      bq2.DeleteMin();
    cout << endl;
  }
	
  FlagNonZero3(input_filename);
}

// Part A for nonzero flag (add numbers into two queues, then merge them)
// uses provided methods
void FlagNonZero(string & input_filename) {
  cout << "PART A: INSERT WITH PROVIDED METHODS" << endl;
	
  char split_char = '_';
  vector<string> temp;
  istringstream split(input_filename);
  for (string str; getline(split, str, split_char); temp.push_back(str));
  int size = stoi(temp[0]);
  int quarter = size/4;
	
  const auto begin = chrono::high_resolution_clock::now();

  int sum = 0;
  for (int i = 1; i < 10000; ++i) sum ++;	
	
  ifstream fin;
  fin.open(input_filename);

  BinomialQueue<int> bq1;
  int num;
  for (int i = 0; i < quarter; ++i) {
    fin >> num;
    bq1.Insert(num);
  }
	
  BinomialQueue<int> bq2;
  while (fin >> num)
    bq2.Insert(num);
	
  bq2.Merge(bq1);
	
  const auto end = chrono::high_resolution_clock::now();
  cout << chrono::duration_cast<chrono::nanoseconds>(end-begin).count() << "ns" << endl;
  cout << chrono::duration_cast<chrono::milliseconds>(end-begin).count() << "ms" << endl;

  fin.close();
  
  // if check to avoid an error of the queue contains fewer than 10 elements
  if (bq2.CurrentSize() >= 10) {
    for (int i = 0; i < 10; ++i)
      bq2.DeleteMin();
    cout << endl;
  } else {
	  while (bq2.CurrentSize() > 0)
      bq2.DeleteMin();
    cout << endl;
  }
	
  FlagNonZero2(input_filename);
}

void TestBinomialQueue(string & input_filename, int flag) {
  cout << "Input is " << input_filename << endl;
  cout << "Flag is " << flag << endl << endl;

  if (flag == 0)
    FlagZero(input_filename);
  else
    FlagNonZero(input_filename);
}

int main(int argc, char **argv) {
  if (argc != 3) {
    cout << "Usage: " << argv[0] << " <input_file_to_create_queue> <flag>" << endl;
    cout << "    flag should be either 0 or 1" << endl;
    return 0;
  }
	
  string input_filename(argv[1]);
  const int flag = atoi(argv[2]);
  
  // check that input file opens successfully
  // program aborts if it does not	
  ifstream fin;
  fin.open(input_filename);
  if (fin.fail()) {
    cout << "File " << input_filename << " failed to open." << endl;
    abort();
  } else {
    fin.close();
    TestBinomialQueue(input_filename, flag);
  }
	
  return 0;
}












