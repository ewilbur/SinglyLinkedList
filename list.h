#ifndef LIST_H_
#define LIST_H_

#include <iostream>
#include <cstdlib>

enum ListException {
  EMPTYLIST,
  INDEXERROR,
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

    /* | Recursively destroys the nodes starting at argument */
    void destroy(Node*);
    /* | helper function to quick sort a list */
    void qsort(Node**, Node*);

  public:
    List();
    List(const List<T>&);
    ~List();

    /* | Prepend to the list */
    void cons(T);
    /* | Append to the list */
    void snoc(T);

    /* | Drops last element from list */
    void init();
    /* Drops first element from list */
    void tail();

    /* | Returns first element of list. Throws exception if empty */
    T head() const;
    /* | Returns last element of list. Throws EMPTYLIST exception if empty */
    T last() const;
    /* | Returns first element of list and removes it. Throws exception if empty */
    T pop_head();
    /* | Returns last element of list and removes it. Throws exception if empty */
    T pop_last();

    /* | Removes first occurence of argument */
    void remove(T);
    /* | Removes all occurences of argument */
    void purge(T);

    /* | Drops the first n elements of the list */
    void drop(size_t);
    /* | Takes the first n elemtns of the list */
    void take(size_t);

    /* | Sorts the list */
    void sort();

    /* | Retuns true iff argument is in the list */
    bool elem(T) const;

    /* | Reverse the order of the list */
    void reverse();

    /* | Returns the length of the list */
    size_t length() const;

    bool operator==(const List<T>&) const;
    bool operator!=(const List<T>&) const;
    bool operator>=(const List<T>&) const;
    bool operator<=(const List<T>&) const;
    bool operator>(const List<T>&) const;
    bool operator<(const List<T>&) const;
    T operator[](size_t) const;
    template <class Type>
    friend std::ostream& operator<<(std::ostream &out, const List<Type> &list) {
      for (Node *step = list.hd; step; step = step->next)
        out << step->datum << ":";
      out << "[]";
      return out;
    }
};

template <class T>
void List<T>::destroy(Node *node) {
  if (!node)
    return;
  destroy(node->next);
  delete node;
}

/* | Use quicksort on elements from *headRef up to last treating the first
 * node as the pivot */
template <class T>
void List<T>::qsort(Node **headRef, Node *last) {
  if (*headRef == last || ! (*headRef))
    return;

  Node *pivot = *headRef;
  Node *step = (*headRef)->next;
  Node **prev = &(*headRef)->next;

  while (step != last) {
    /* | If the datam at step is larger than the pivot, just go to the next
     * element */
    if (step->datum > pivot->datum) {
      prev = &step->next;
      step = step->next;
    } else {
      *prev = step->next;    // Take the node out of the list
      step->next = *headRef; // Put step at the front of the list _
      *headRef = step;       // <---------------------------------/
      step = *prev;          // Iterate to the next element
    }
  }
  qsort(headRef, pivot);     // Recursively sort the rest of the list _
  qsort(&pivot->next, last); // <-------------------------------------/
}

template <class T>
List<T>::List() : hd(0) {}

/* | Not an incredibly efficient copy constructor, but it's terse so
 * I like it */
template <class T>
List<T>::List(const List<T> &list) {
  for (Node *step = list.hd; step; step = step->next)
    cons(step->cons);
  reverse();
}

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
  Node *step = hd;
  if (!hd)
    throw(EMPTYLIST);
  while (step->next)
    step = step->next;
  return step->datum;
}

template <class T>
T List<T>::head() const {
  if (!hd)
    throw(EMPTYLIST);
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

template <class T>
T List<T>::operator[](size_t k) const {
  Node *step = hd;
  for (; step && k > 0; --k, step = step->next);
  if (!step)
    throw(INDEXERROR);

  return step->datum;
}

#endif // LIST_H_
