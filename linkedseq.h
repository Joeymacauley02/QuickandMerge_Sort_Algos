//----------------------------------------------------------------------
// NAME: Joey Macauley
// FILE: linkedlist.h
// DATE: CPSC 223 - Spring 2022
// DESC: Implementation of an LinkedList (Quicksort and Mergesort)
//----------------------------------------------------------------------

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdexcept>
#include <ostream>
#include "sequence.h"

template <typename T>
class LinkedSeq : public Sequence<T>
{
public:
  // Default constructor
  LinkedSeq();

  // Copy constructor
  LinkedSeq(const LinkedSeq &rhs);

  // Move constructor
  LinkedSeq(LinkedSeq &&rhs);

  // Copy assignment operator
  LinkedSeq &operator=(const LinkedSeq &rhs);

  // Move assignment operator
  LinkedSeq &operator=(LinkedSeq &&rhs);

  // Destructor
  ~LinkedSeq();

  // Returns the number of elements in the sequence
  int size() const override;

  // Tests if the sequence is empty
  bool empty() const override;

  // Removes all of the elements from the sequence
  void clear() override;

  // Returns a reference to the element at the index in the
  // sequence. Throws out_of_range if index is invalid.
  T &operator[](int index) override;

  // Returns a constant address to the element at the index in the
  // sequence. Throws out_of_range if index is invalid.
  const T &operator[](int index) const override;

  // Extends the sequence by inserting the element at the given
  // index. Throws out_of_range if the index is invalid.
  void insert(const T &elem, int index) override;

  // Shrinks the sequence by removing the element at the index in the
  // sequence. Throws out_of_range if index is invalid.
  void erase(int index) override;

  // Returns true if the element is in the sequence, and false
  // otherwise.
  bool contains(const T &elem) const override;

  // Sorts the elements in the sequence in place using less than equal
  // (<=) operator. Uses merge sort.
  void sort();

  // Sorts the sequence in place using the merge sort algorithm.
  void merge_sort();

  // Sorts the sequence in place using the quick sort algorithm. Uses
  // first element for pivot values.
  void quick_sort();

  // Sorts the sequence in place using the quick sort algorithm. Uses
  // randomly selected indexes for pivot values.
  void quick_sort_random();

private:
  // linked list node
  struct Node
  {
    T value;
    Node *next = nullptr;
  };

  // head pointer
  Node *head = nullptr;

  // tail pointer
  Node *tail = nullptr;

  // size of list
  int node_count = 0;

  // sort function helpers
  Node *merge_sort(Node *left, int len);
  Node *quick_sort(Node *start, int len);
  Node *quick_sort_random(Node *start, int len);

  // random seed for quick sort
  int seed = 22;
};

template <typename T>
std::ostream &operator<<(std::ostream &stream, const LinkedSeq<T> &seq)
{
  int n = seq.size();
  for (int i = 0; i < n - 1; ++i)
    stream << seq[i] << ", ";
  if (n > 0)
    stream << seq[n - 1];
  return stream;
}

template <typename T>
LinkedSeq<T>::LinkedSeq()
{
}

template <typename T>
LinkedSeq<T>::LinkedSeq(const LinkedSeq<T> &rhs)
{
  *this = rhs;
}

template <typename T>
LinkedSeq<T>::LinkedSeq(LinkedSeq<T> &&rhs)
{
  *this = std::move(rhs);
}

template <typename T>
LinkedSeq<T> &LinkedSeq<T>::operator=(const LinkedSeq<T> &rhs)
{
  if (this != &rhs)
  {
    Node *temp = rhs.head;
    clear();
    while (temp != nullptr)
    {
      insert(temp->value, node_count);
      temp = temp->next;
    }
  }
  return *this;
}

template <typename T>
LinkedSeq<T> &LinkedSeq<T>::operator=(LinkedSeq<T> &&rhs)
{
  if (this != &rhs)
  {
    clear();
    head = rhs.head;
    tail = rhs.tail;
    node_count = rhs.node_count;

    rhs.head = nullptr;
    rhs.tail = nullptr;
    rhs.node_count = 0;
  }
  return *this;
}

template <typename T>
LinkedSeq<T>::~LinkedSeq()
{
  clear();
}

template <typename T>
bool LinkedSeq<T>::empty() const
{
  if (head == nullptr)
  {
    return true;
  }
  return false;
}

template <typename T>
void LinkedSeq<T>::clear()
{
  Node *temp = head;
  Node *nextPtr = nullptr;

  while (temp != nullptr)
  {
    nextPtr = temp->next;
    delete temp;
    temp = nextPtr;
    node_count--;
  }
  head = nullptr;
  tail = nullptr;
}

template <typename T>
T &LinkedSeq<T>::operator[](int index)
{
  Node *update = nullptr;
  if (index < 0 || index >= size())
  {
    throw std::out_of_range("Invalid index");
  }
  else if (index == size() - 1)
  {
    update = tail;
  }
  else
  {
    Node *currNode = head;
    for (int i = 0; i < index; ++i)
    {
      currNode = currNode->next;
    }
    update = currNode;
  }
  return update->value;
}

template <typename T>
const T &LinkedSeq<T>::operator[](int index) const
{
  Node *nodePtr = nullptr;
  if (index < 0 || index >= size())
  {
    throw std::out_of_range("Invalid index");
  }
  else if (index == size() - 1)
  {
    nodePtr = tail;
  }
  else
  {
    Node *currNode = head;
    for (int i = 0; i < index; ++i)
    {
      currNode = currNode->next;
    }
    nodePtr = currNode;
  }
  return nodePtr->value;
}

template <typename T>
void LinkedSeq<T>::insert(const T &elem, int index)
{
  if (index < 0 || index > size())
  {
    throw std::out_of_range("Invalid index");
  }

  Node *newnode = new Node;
  newnode->value = elem;
  if (empty()) // List is empty
  {
    head = newnode;
    tail = newnode;
    ++node_count;
  }
  else if (index == 0)
  {
    newnode->next = head;
    head = newnode;
    ++node_count;
  }
  else if (index == size()) // Add to the end of the list
  {
    tail->next = newnode;
    tail = newnode;
    ++node_count;
  }
  else // Add node at certain index
  {
    Node *currNode = head;
    for (int i = 0; i < index - 1; ++i)
    {
      currNode = currNode->next;
    }
    newnode->next = currNode->next;
    currNode->next = newnode;
    ++node_count;
  }
}

template <typename T>
void LinkedSeq<T>::erase(int index)
{
  Node *traverseNode = head;
  Node *removeNode = nullptr;

  if (index < 0 || index >= size())
  {
    throw std::out_of_range("Invalid index");
  }
  else if (index == 0)
  {
    removeNode = head;
    head = removeNode->next;
    delete removeNode;
    node_count--;
  }
  else
  {
    for (int i = 0; i < index - 1; ++i)
    {
      traverseNode = traverseNode->next;
    }
    removeNode = traverseNode->next;
    traverseNode->next = removeNode->next;
    delete removeNode;
    node_count--;
  }
}

template <typename T>
bool LinkedSeq<T>::contains(const T &elem) const
{
  Node *traverseNode = head;
  if (empty())
  {
    return false;
  }
  else
  {
    while (traverseNode != nullptr)
    {
      if (traverseNode->value == elem)
      {
        return true;
      }
      traverseNode = traverseNode->next;
    }
  }
  return false;
}

template <typename T>
int LinkedSeq<T>::size() const
{
  return node_count;
}

// Helper Functions
template <typename T>
void LinkedSeq<T>::sort()
{
  merge_sort();
}

template <typename T>
void LinkedSeq<T>::merge_sort()
{
  head = merge_sort(head, size());

  if (head == nullptr)
  {
    tail = head;
  }
  else
  {
    Node *traverse = head;
    while (traverse->next != nullptr)
    {
      traverse = traverse->next;
    }
    tail = traverse;
  }
}

template <typename T>
void LinkedSeq<T>::quick_sort()
{
  head = quick_sort(head, size());

  if (head == nullptr)
  {
    tail = head;
  }
  else
  {
    Node *traverse = head;
    while (traverse->next != nullptr)
    {
      traverse = traverse->next;
    }
    tail = traverse;
  }
}

template <typename T>
void LinkedSeq<T>::quick_sort_random()
{
  std::srand(seed);
  head = quick_sort_random(head, size());

  if (head == nullptr)
  {
    tail = head;
  }
  else
  {
    Node *traverse = head;
    while (traverse->next != nullptr)
    {
      traverse = traverse->next;
    }
    tail = traverse;
  }
}

template <typename T>
typename LinkedSeq<T>::Node *LinkedSeq<T>::merge_sort(Node *left, int len)
{
  int mid = 0;
  if (len <= 1)
  {
    return left;
  }
  else
  {
    Node *right = nullptr;
    Node *traverse = left;
    mid = len / 2;
    for (int i = 0; i < mid - 1; ++i)
    {
      traverse = traverse->next;
    }
    right = traverse->next;
    traverse->next = nullptr;

    left = merge_sort(left, mid);
    right = merge_sort(right, (len - mid));

    Node *front = nullptr;
    Node *end = nullptr;

    if (!left) // left is empty
    {
      return right;
    }
    else if (!right) // right is empty
    {
      return left;
    }

    // setting a head pointer
    if (left->value <= right->value)
    {
      front = left;
      left = left->next;
    }
    else
    {
      front = right;
      right = right->next;
    }
    // setting a tail pointer
    end = front;

    // traversing list and comparing
    while (left and right)
    {
      Node *hold = nullptr;

      if (left->value <= right->value)
      {
        hold = left;
        left = left->next;
      }
      else
      {
        hold = right;
        right = right->next;
      }

      end->next = hold;
      end = end->next;
    }

    // Add to end if necessary
    if (left)
    {
      end->next = left;
    }
    else if (right)
    {
      end->next = right;
    }

    return front;
  }
}

template <typename T>
typename LinkedSeq<T>::Node *LinkedSeq<T>::quick_sort(Node *start, int len)
{
  if (len <= 1)
  {
    return start;
  }
  else
  {
    // take first node AND detach it from the list
    Node *pivot = start;
    start = pivot->next;
    pivot->next = nullptr;

    // small list
    Node *smaller = nullptr;     // head
    Node *smaller_end = nullptr; // tail
    Node *larger = nullptr;      // head
    Node *larger_end = nullptr;  // tail
    int smaller_len = 0, larger_len = 0;

    Node *hold = nullptr;

    while (start != nullptr)
    {
      if (start->value <= pivot->value)
      {
        hold = start; // is this necessary
        start = hold->next;
        hold->next = nullptr;

        if (smaller == nullptr)
        {
          smaller = hold;
          smaller_end = smaller;
        }
        else
        {
          smaller_end->next = hold;
          smaller_end = smaller_end->next;
        }
        smaller_len++;
      }
      else
      {
        hold = start;
        start = hold->next;
        hold->next = nullptr;

        if (larger == nullptr)
        {
          larger = hold;
          larger_end = larger;
        }
        else
        {
          larger_end->next = hold;
          larger_end = larger_end->next;
        }
        larger_len++;
      }
    }

    smaller = quick_sort(smaller, smaller_len);
    larger = quick_sort(larger, larger_len);

    pivot->next = larger;

    // Pivot was the smallest
    if (!smaller)
    {
      return pivot;
    }
    // Find smaller tail, add pivot/larger
    else
    {
      hold = smaller;
      while (hold->next != nullptr)
      {
        hold = hold->next;
      }
      hold->next = pivot;
      return smaller;
    }
  }
}

template <typename T>
typename LinkedSeq<T>::Node *LinkedSeq<T>::quick_sort_random(Node *start, int len)
{
  if (len <= 1)
  {
    return start;
  }
  else
  {
    int randIdx = rand() % (len - 1);
    int pivotVal = 0, holdVal = 0;
    Node *pivot = start;
    for (int i = 0; i < randIdx; ++i)
    {
      pivot = pivot->next;
    }
    pivotVal = pivot->value;
    // swap values of pivot and start
    holdVal = start->value;
    start->value = pivotVal;
    pivot->value = holdVal;

    // take first node AND detach it from the list
    pivot = start;
    start = pivot->next;
    pivot->next = nullptr;

    Node *smaller = nullptr;     // head
    Node *smaller_end = nullptr; // tail
    Node *larger = nullptr;      // head
    Node *larger_end = nullptr;  // tail
    int smaller_len = 0, larger_len = 0;

    Node *hold = nullptr;

    while (start != nullptr)
    {
      if (start->value <= pivot->value)
      {
        hold = start; // is this necessary
        start = hold->next;
        hold->next = nullptr;

        if (smaller == nullptr)
        {
          smaller = hold;
          smaller_end = smaller;
        }
        else
        {
          smaller_end->next = hold;
          smaller_end = smaller_end->next;
        }
        smaller_len++;
      }
      else
      {
        hold = start;
        start = hold->next;
        hold->next = nullptr;

        if (larger == nullptr)
        {
          larger = hold;
          larger_end = larger;
        }
        else
        {
          larger_end->next = hold;
          larger_end = larger_end->next;
        }
        larger_len++;
      }
    }

    smaller = quick_sort_random(smaller, smaller_len);
    larger = quick_sort_random(larger, larger_len);

    pivot->next = larger;

    // Pivot was the smallest
    if (!smaller)
    {
      return pivot;
    }
    // Find smaller tail, add pivot/larger
    else
    {
      hold = smaller;
      while (hold->next != nullptr)
      {
        hold = hold->next;
      }
      hold->next = pivot;
      return smaller;
    }
  }
}

#endif
