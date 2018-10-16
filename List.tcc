//
// Created by Bazyli on 05.03.18.
//

#include <iostream>
#include "List.h"

using namespace std;

/**
 * Creates empty list.
 *
 * @tparam T Type of the data held in this list.
 */
template<class T>
List<T>::List() = default;

/**
 * Copy constructor. Copies all elements from the given list.
 *
 * @tparam T Type of the data held in this list.
 * @param src List to copy.
 */
template<class T>
List<T>::List(const List<T> &src) {
    for (int i = 0; i < src.getLength(); i++) {
        pushBack(src.get(i));
    }
}

/**
 * Frees up memory by deleting all list elements.
 *
 * @tparam T Type of the data held in this list.
 */
template<class T>
List<T>::~List() {
    ListElement<T> *tmp = head;
    ListElement<T> *next;

    while (tmp != nullptr) {
        next = tmp->getNext();
        delete tmp;
        tmp = next;
    }
}

/**
 * Assignment operator.
 *
 * @tparam T Type of the data held in this list.
 * @param src Assigned list.
 * @return New list.
 */
template<class T>
List<T> &List<T>::operator=(List<T> src) {
    swap(head, src.head);
    swap(tail, src.tail);
    swap(length, src.length);

    return *this;
}

/**
 * Sets element of list with given key to given value.
 *
 * @tparam T Type of the data held in this list.
 * @param key Index of element to set.
 * @param value Value to set.
 */
template<class T>
void List<T>::set(int key, T value) {
    if (key < 0) {
        throw out_of_range("Requested index out out of list bounds.");
    }

    ListElement<T> *tmp = head;

    for (int i = 0; i < key; i++) {
        if (!tmp || !tmp->getNext()) {
            throw out_of_range("Requested index out out of list bounds.");
        }
        tmp = tmp->getNext();
    }

    tmp->setValue(value);
}

/**
 * Returns value of element with given index.
 *
 * @tparam T Type of the data held in this list.
 * @param key Index of element to retrieve.
 */
template<class T>
T List<T>::get(int key) const {
    if (key < 0) {
        throw out_of_range("Requested index out out of list bounds.");
    }

    ListElement<T> *tmp = head;

    for (int i = 0; i < key; i++) {
        if (!tmp || !tmp->getNext()) {
            throw out_of_range("Requested index out out of list bounds.");
        }
        tmp = tmp->getNext();
    }

    return tmp->getValue();
}

/**
 * Adds element with given value on given index.
 *
 * @tparam T Type of the data held in this list.
 * @param key Index where element will be added.
 * @param value Value of element to add.
 */
template<class T>
void List<T>::insert(int key, T value) {
    if (key < 0) {
        throw out_of_range("Requested index out out of list bounds.");
    }

    ListElement<T> *tmp = head;
    auto el = new ListElement<T>(value);

    for (int i = 0; i < key; i++) {
        if (!tmp || !tmp->getNext()) {
            throw out_of_range("Requested index out out of list bounds.");
        }

        tmp = tmp->getNext();
    }

    el->setPrev(tmp->getPrev());
    el->setNext(tmp);

    if (!tmp->getPrev()) {
        head = el;
    } else {
        tmp->getPrev()->setNext(el);
    }

    tmp->setPrev(el);
    length++;
}

/**
 * Adds element before the first element of the list.
 *
 * @tparam T Type of the data held in this list.
 * @param value Value to add.
 */
template<class T>
void List<T>::pushFront(T value) {
    auto el = new ListElement<T>(value);

    el->setNext(head);

    if (head != nullptr) {
        head->setPrev(el);
    }

    if (tail == nullptr) {
        tail = el;
    }

    head = el;
    length++;
}

/**
 * Adds element with given value to the end of the list.
 *
 * @tparam T Type of the data held in this list.
 * @param value Value to add.
 */
template<class T>
void List<T>::pushBack(T value) {
    auto el = new ListElement<T>(value);

    el->setPrev(tail);
    el->setNext(nullptr);

    if (head == nullptr) {
        head = el;
    }

    if (tail != nullptr) {
        tail->setNext(el);
    }

    tail = el;
    length++;
}

/**
 * Removes element with given key.
 *
 * @tparam T Type of the data held in this list.
 * @param key Index of element to remove.
 */
template<class T>
void List<T>::remove(int key) {
    if (key < 0) {
        throw out_of_range("Requested index out out of list bounds.");
    }

    ListElement<T> *tmp = head;

    for (int i = 0; i < key; i++) {
        if (!tmp || !tmp->getNext()) {
            throw out_of_range("Requested index out out of list bounds.");
        }
        tmp = tmp->getNext();
    }

    if (!tmp) {
        throw out_of_range("Requested index out out of list bounds.");
    }

    if (tmp->getPrev()) {
        tmp->getPrev()->setNext(tmp->getNext());
    } else {
        head = tmp->getNext();
    }

    if (tmp->getNext()) {
        tmp->getNext()->setPrev(tmp->getPrev());
    } else {
        tail = tmp->getPrev();
    }

    delete tmp;
    length--;
}

/**
 * Removes first element of the list.
 *
 * @tparam T Type of the data held in this list.
 */
template<class T>
void List<T>::popFront() {
    if (!head) {
        throw out_of_range("Requested index out out of list bounds.");
    }

    if (head->getNext()) {
        head = head->getNext();
        delete head->getPrev();
        head->setPrev(nullptr);
    } else {
        delete head;
        head = nullptr;
        tail = nullptr;
    }
    length--;
}

/**
 * Removes last element of the list.
 *
 * @tparam T Type of the data held in this list.
 */
template<class T>
void List<T>::popBack() {
    if (!tail) {
        throw out_of_range("Requested index out out of list bounds.");
    }

    if (tail->getPrev()) {
        tail = tail->getPrev();
        delete tail->getNext();
        tail->setNext(nullptr);
    } else {
        delete tail;
        head = nullptr;
        tail = nullptr;
    }
    length--;
}

/**
 * @tparam T Type of the data held in this list.
 * @return Length of the list.
 */
template<class T>
int List<T>::getLength() const {
    return length;
}

/**
 * Prints values stored in this List.
 *
 * @tparam T Type of the data held in this list.
 */
template<class T>
void List<T>::print() {
    ListElement<T> *tmp = head;
    while (tmp != nullptr) {
        cout << tmp->getValue() << ", ";
        tmp = tmp->getNext();
    }
    cout << endl;
}
