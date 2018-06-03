#ifndef LIST_H_
#define LIST_H_

#include <iostream>
#include <cstdlib>

enum ListException {
  EMPTYLIST
};

template <class T>
class List {
  private:
    struct Node {
      T datum;
      Node *next;
      Node(T datum) : datum(datum), next(0) {}
      Node(T datum, Node *next) : datum(datum), next(next) {}
    } *hd;

    void destroy(Node*); // Recursively destroys the nodes starting at argument
    void qsort(Node**, Node*); // helper function to quick sort a list

  public:
    List();
    ~List();

    /* | Prepend to the list */
    void cons(T);
    /* | Append to the list */
    void snoc(T);

    void init(); // Drops last element from list
    void tail(); // Drops first element from list

    T head() const; // Returns first element of list. Throws exception if empty
    T last() const; // Returns last element of list. Throws exception if empty
    T pop_head(); // Returns first element of list and removes it. Throws exception if empty
    T pop_last(); // Returns last element of list and removes it. Throws exception if empty

    void remove(T); // Removes first occurence of argument
    void purge(T); // Removes all occurences of argument

    void drop(size_t); // Drops the first n elements of the list
    void take(size_t); // Takes the first n elemtns of the list

    void sort(); // Sorts the list

    bool elem(T) const; // Retuns true iff argument is in the list

    void reverse(); // Reverse the order of the list

    void print(); // Prints the contents of the list

    size_t length() const; // Returns the length of the list

    bool operator==(const List<T>&) const;
    bool operator!=(const List<T>&) const;
    bool operator>=(const List<T>&) const;
    bool operator<=(const List<T>&) const;
    bool operator>(const List<T>&) const;
    bool operator<(const List<T>&) const;
};


template <class T>
void List<T>::destroy(Node *node) {
  if (!node)
    return;
  destroy(node->next);
  delete node;
}

template <class T>
void List<T>::qsort(Node **headRef, Node *last) {
  if (*headRef == last || ! (*headRef)) return;

  Node *pivot = *headRef;
  Node *step = (*headRef)->next;
  Node **prev = &(*headRef)->next;

  while (step != last) {
    if (step->datum > pivot->datum) {
      prev = &step->next;
      step = step->next;
    } else {
      *prev = step->next;
      step->next = *headRef;
      *headRef = step;
      step = *prev;
    }
  }
  qsort(headRef, pivot);
  qsort(&pivot->next, last);
}

template <class T>
List<T>::List() : hd(0) {}


template <class T>
List<T>::~List() {
  destroy(hd);
}

template <class T>
void List<T>::cons(T datum) {
  Node *newHead = new Node(datum, hd);
  hd = newHead;
}

template <class T>
void List<T>::snoc(T datum) {
  Node *last = new Node(datum);
  Node **step = &hd;
  while (*step)
    step = &(*step)->next;
  *step = last;
}

template <class T>
void List<T>::init() {
  Node *node = hd;
  Node **prev = &hd;
  while (node && node->next) {
    prev = &node->next;
    node = node->next;
  }
  if (node)
    delete node;
  *prev = 0;
}


template <class T>
void List<T>::tail() {
  if (hd) {
    Node *newHead = hd->next;
    delete hd;
    hd = newHead;
  }
}

template <class T>
T List<T>::pop_head() {
  T x = head();
  tail();
  return x;
}

template <class T>
T List<T>::pop_last() {
  T x = last();
  init();
  return x;
}

template <class T>
T List<T>::last() const {
  if (!hd) throw(EMPTYLIST);
  Node *step = hd;
  while (step->next)
    step = step->next;
  return step->datum;
}

template <class T>
T List<T>::head() const {
  if (!hd) throw(EMPTYLIST);
  return hd->datum;
}

template <class T>
void List<T>::remove(T datum) {
  Node **prev = &hd;
  Node *step = hd;
  while (step) {
    if (step->datum == datum) {
      *prev = step->next;
      delete step;
      return;
    } else {
      prev = &step->next;
      step = step->next;
    }
  }
}

template <class T>
void List<T>::purge(T datum) {
  Node **prev = &hd;
  Node *step = hd;
  while (step) {
    if (step->datum == datum) {
      *prev = step->next;
      delete step;
      step = *prev;
    } else {
      prev = &step->next;
      step = step->next;
    }
  }
}

template <class T>
void List<T>::drop(size_t k) {
  Node *step = hd;
  Node *prev;
  while (step && k > 0) {
    prev = step->next;
    delete step;
    step = prev;
    --k;
  }
  hd = step;
}

template <class T>
void List<T>::take(size_t k) {
  Node **prev = &hd;
  Node *step = hd;
  while (step && k > 0) {
    prev = &step->next;
    step = step->next;
    --k;
  }
  destroy(step);
  *prev = 0;
}

template <class T>
void List<T>::sort() {
  qsort(&hd,0);
}

template <class T>
bool List<T>::elem(T datum) const {
  for (Node *step = hd; step; step = step->next)
    if (step->datum == datum)
      return true;
  return false;
}

template <class T>
void List<T>::reverse() {
  Node *step = hd;
  Node *prev = 0;
  while (step) {
    Node *next = step->next;
    step->next = prev;
    prev = step;
    step = next;
  }
  hd = prev;
}

template <class T>
size_t List<T>::length() const {
  int i = 0;
  for (Node *step = hd; step; step = step->next, ++i);
  return i;
}

template <class T>
void List<T>::print() {
  for (Node *step = hd; step; step = step->next)
    std::cout << step->datum << ":";

  std::cout << "[]" << std::endl;
}

template <class T>
bool List<T>::operator==(const List<T> &list) const {
  Node *list1 = hd;
  Node *list2 = list.hd;

  for (; list1 && list2; list1 = list1->next, list2 = list2->next)
    if (list1->datum != list2->datum)
      return false;
  return list1 == list2;
}

template <class T>
bool List<T>::operator!=(const List<T> &list) const {
  return !(*this == list);
}

template <class T>
bool List<T>::operator>=(const List<T> &list) const {
  Node *list1 = hd;
  Node *list2 = list.hd;

  for (; list1 && list2; list1 = list1->next, list2 = list2->next) {
    if (list1->datum > list2->datum)
      return true;
    else if (list1->datum < list2->datum)
      return false;
  }
  return (list1 == list2) || !list2;
}

template <class T>
bool List<T>::operator<=(const List<T> &list) const {
  return list >= *this;
}

template <class T>
bool List<T>::operator>(const List<T> &list) const {
  Node *list1 = hd;
  Node *list2 = list.hd;

  for (; list1 && list2; list1 = list1->next, list2 = list2->next) {
    if (list1->datum > list2->datum)
      return true;
    else if (list1->datum < list2->datum)
      return false;
  }
  return (list1 != list2) && !list2;
}

template <class T>
bool List<T>::operator<(const List<T> &list) const {
  return list > *this;
}

#endif // LIST_H_
