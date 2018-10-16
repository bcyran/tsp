//
// Created by Bazyli on 05.03.18.
//

#ifndef PROJEKT1_LIST_H
#define PROJEKT1_LIST_H


#include "ListElement.h"

/**
 * Doubly linked list.
 *
 * @tparam T Type of the data held in this list.
 */
template<class T>
class List {

    /** Pointer to the first element of the list. */
    ListElement<T> *head = nullptr;

    /** Pointer to the last element of the list. */
    ListElement<T> *tail = nullptr;

    /** Length of the list. */
    int length = 0;

public:

    List();

    List(const List<T> &src);

    virtual ~List();

    List<T> &operator=(List<T> src);

    void set(int key, T value);

    T get(int key) const;

    void insert(int key, T value);

    void pushFront(T value);

    void pushBack(T value);

    void remove(int key);

    void popFront();

    void popBack();

    void print();

    int getLength() const;

};

#include "List.tcc"

#endif //PROJEKT1_LIST_H
