//---------------------------------------------------------------------------
// NAME: S. Bowers
// FILE: hw4_perf.cpp
// DATE: Fall 2021
// DESC: Simple performance test driver for HW-4. To run from the
//       command line use:
//          ./hw4_perf
//       which will print out timing data resulting from running merge
//       sort and quick sort over different sized sorted, reverse
//       sorted, and shuffled resizable array and linked list
//       sequences. To save this data to a file, run the command:
//          ./hw4_perf > output.dat
//       This file can then be used by the plotting script to generate
//       the corresponding performance graphs.
//---------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <chrono>
#include <functional>
#include "util.h"
#include "sequence.h"
#include "arrayseq.h"
#include "linkedseq.h"


using namespace std;
using namespace std::chrono;

typedef function<void(ArraySeq<int>&)> array_sort_fn;
typedef function<void(LinkedSeq<int>&)> linked_sort_fn;

// helper functions for sorting
void array_merge_sort(ArraySeq<int>& s)
{
  s.merge_sort();
}

void array_quick_sort(ArraySeq<int>& s)
{
  s.quick_sort();
}

void array_quick_sort_random(ArraySeq<int>& s)
{
  s.quick_sort_random();
}

void linked_merge_sort(LinkedSeq<int>& s)
{
  s.merge_sort();
}

void linked_quick_sort(LinkedSeq<int>& s)
{
  s.quick_sort();
}

void linked_quick_sort_random(LinkedSeq<int>& s)
{
  s.quick_sort_random();
}

// helper functions for timing and simple sort check
double array_timed(const ArraySeq<int>& seq, array_sort_fn f);
double linked_timed(const LinkedSeq<int>& seq, linked_sort_fn f);
void check_sorted(const Sequence<int>& s);

// test parameters
const int start = 0;
const int step = 1500; 
const int stop = 15000;
const int runs = 1;
const int shuffles = 5;


int main(int argc, char* argv[])
{
  // configure output
  cout << fixed << showpoint;
  cout << setprecision(2);

  // output data header
  cout << "# All times in milliseconds (msec)" << endl;
  cout << "# Column 1 = input data size" << endl;

  cout << "# Column 2 = avg time array merge sort, reversed" << endl;
  cout << "# Column 3 = avg time array merge sort, shuffled" << endl;

  cout << "# Column 4 = avg time array quick sort, reversed" << endl;
  cout << "# Column 5 = avg time array quick sort, shuffled" << endl;

  cout << "# Column 6 = avg time array quick sort random, reversed" << endl;
  cout << "# Column 7 = avg time array quick sort random, shuffled" << endl;

  cout << "# Column 8 = avg time linked merge sort, reversed" << endl;
  cout << "# Column 9 = avg time linked merge sort, shuffled" << endl;

  cout << "# Column 10 = avg time linked quick sort, reversed" << endl;
  cout << "# Column 11 = avg time linked quick sort, shuffled" << endl;

  cout << "# Column 12 = avg time linked quick sort random, reversed" << endl;
  cout << "# Column 13 = avg time linked quick sort random, shuffled" << endl;

  
  // run tests and print test results
  for (int size = start; size <= stop; size += step) {

    ArraySeq<int> array_reversed, array_shuffled;
    LinkedSeq<int> linked_reversed, linked_shuffled;
    load_reverse_order(array_reversed, size);
    load_shuffled(array_shuffled, size, shuffles);
    load_reverse_order(linked_reversed, size);
    load_shuffled(linked_shuffled, size, shuffles);

    double c2 = array_timed(array_reversed, array_merge_sort);
    double c3 = array_timed(array_shuffled, array_merge_sort);
    
    double c4 = array_timed(array_reversed, array_quick_sort);
    double c5 = array_timed(array_shuffled, array_quick_sort);

    double c6 = array_timed(array_reversed, array_quick_sort_random);
    double c7 = array_timed(array_shuffled, array_quick_sort_random);

    double c8 = linked_timed(linked_reversed, linked_merge_sort);
    double c9 = linked_timed(linked_shuffled, linked_merge_sort);

    double c10 = linked_timed(linked_reversed, linked_quick_sort);
    double c11 = linked_timed(linked_shuffled, linked_quick_sort);

    double c12 = linked_timed(linked_reversed, linked_quick_sort_random);
    double c13 = linked_timed(linked_shuffled, linked_quick_sort_random);

    cout << size << " " << c2 << " " << c3 << " " << c4 << " "
	 << c5 << " " << c6 << " " << c7 << " " << c8 << " "
	 << c9 << " " << c10 << " " << c11 << " " << c12 << " "
         << c13 << endl;
  }

}

double array_timed(const ArraySeq<int>& seq, array_sort_fn f)
{
  int total = 0;
  for (int r = 0; r < runs; ++r) {
    ArraySeq<int> s = seq;
    auto t0 = high_resolution_clock::now();
    f(s);
    auto t1 = high_resolution_clock::now();
    total += duration_cast<milliseconds>(t1 - t0).count();
    check_sorted(s);
  }
  return (total * 1.0) / runs;
}

double linked_timed(const LinkedSeq<int>& seq, linked_sort_fn f)
{
  int total = 0;
  for (int r = 0; r < runs; ++r) {
    LinkedSeq<int> s = seq;
    auto t0 = high_resolution_clock::now();
    f(s);
    auto t1 = high_resolution_clock::now();
    total += duration_cast<milliseconds>(t1 - t0).count();
    check_sorted(s);
  }
  return (total * 1.0) / runs;
}

void check_sorted(const Sequence<int>& s)
{
  for (int i = 0; i < s.size() - 1; ++i) {
    if (s[i] > s[i+1]) {
      std::cerr << "Error: Sequence not sorted: s[" << i << "] = "
                << s[i] << " > " << "s[" << (i + 1) << "] = "
                << s[i+1] << endl;
      std::terminate();
    }
  }
}
