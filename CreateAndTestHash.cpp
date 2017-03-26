// Dina Mangialino
// CSCI 335 Assignment 3 - Hashing and Heaps
// March 26, 2016

// CreateAndTestHash.cpp
// Reads words from a file and adds them to a hash table with collision resolution method dependent on user input.
// Calculates number of elements in the table, table size, load factor, number of collisions, and average number of collisions in the table.
// Searches for words from query words file and reports whether or not the word is found and the number of probes taken to search for the word.

#include "QuadraticProbing.h"
#include "SeparateChaining.h"
#include "LinearProbing.h"
#include "DoubleHashing.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

template <typename HashTableType>
void TestFunctionForHashTable(HashTableType &hash_table, const string &words_filename, const string &query_filename) {
  ifstream fin;
  fin.open(words_filename);
  string word;
	
  while (fin >> word) {
    hash_table.Insert(word);
  }
	
  fin.close();
	
  //total number of elements in the table (N)
  double n = hash_table.CurrentSize();
  cout << "Elements in the Table (N):" << '\t' << n << endl;

  //size of the table (T)
  double t = hash_table.TableSize();
  cout << "Size of the Table (T): " << '\t' << '\t' << t << endl;

  //load factor (N/T)
  cout << "Load Factor (N/T):" << '\t' << '\t' << n/t << endl;

  //total number of collisions (C)
  float c = hash_table.CollisionCount();
  cout << "Collisions (C):" << '\t' << '\t' << '\t' << c << endl;
	
  //average number of collisions (C/N)
  cout << "Avg Number of Collisions (C/N):" << '\t' << c/n << endl;
	
  cout << endl << endl;
	
  fin.open(query_filename);
  int probes = 0;
  while (fin >> word) {
    if(hash_table.Contains(word, probes) == true)
      if(word.length() < 10)
        cout << word << '\t' << '\t' << "Found" << '\t' << '\t' << probes << endl;
      else
        cout << word << '\t' << "Found" << '\t' << '\t' << probes << endl;
    else
      if(word.length() < 10)
        cout << word << '\t' << '\t' << "Not Found" << '\t' << probes << endl;
      else
        cout << word << '\t' << "Not Found" << '\t' << probes << endl;
  }

  fin.close();
}

int main(int argc, char **argv) {
  if (argc != 4) {
    cout << "Usage: " << argv[0] << " <wordsfilename> <queryfilename> <flag>" << endl;
    return 0;
  }
  
  const string words_filename(argv[1]);
  const string query_filename(argv[2]);
  const string param_flag(argv[3]);
  cout << "Input words file is " << words_filename << ", and query file is " << query_filename << endl;
	
  // check that the file with the words opens successfully	
  // program aborts if it does not	
  ifstream fin;
  fin.open(words_filename);
  if (fin.fail()) {
    cout << "File " << words_filename << " failed to open." << endl;
    abort();
  } else {
    fin.close();
  }

  // check that the file with query words opens successfully
  // program aborts if it does not
  fin.open(query_filename);
  if (fin.fail()) {
    cout << "File " << query_filename << " failed to open." << endl;
    abort();
  } else {
    fin.close();
  }
	
  if (param_flag == "linear") {
    cout << "Hash Table with Linear Probing" << endl << endl;
    LinearProbHashTable<string> linear_probing_table;
    TestFunctionForHashTable(linear_probing_table, words_filename, query_filename);    		
  } else if (param_flag == "quadratic") {
    cout << "Hash Table with Quadratic Probing" << endl << endl;
    HashTable<string> quadratic_probing_table;
    TestFunctionForHashTable(quadratic_probing_table, words_filename, query_filename);    
  } else if (param_flag == "double") {
    cout << "Hash Table with Double Hashing" << endl << endl;
    DoubleHashTable<string> double_hash_table;
    TestFunctionForHashTable(double_hash_table, words_filename, query_filename);
  } else if (param_flag == "chaining") {
    cout << "Hash Table with Separate Chaining" << endl << endl;
    SepChainHashTable<string> separate_chaining_table;
    TestFunctionForHashTable(separate_chaining_table, words_filename, query_filename);
  } else {
    cout << "Unknown collision resolution type " << param_flag << endl;
    cout << "(User should provide linear, quadratic, chaining, or double)" << endl << endl;
  }
	
  return 0;
}
