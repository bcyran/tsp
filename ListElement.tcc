//
// Created by Bazyli on 19.03.18.
//

#include "ListElement.h"

/**
 * Creates new empty list element.
 *
 * @tparam T Type of the data held by this element.
 */
template<class T>
ListElement<T>::ListElement() = default;

/**
 * Creates new list element with given value.
 *
 * @tparam T Type of the data held by this element.
 * @param value Value to set.
 */
template<class T>
ListElement<T>::ListElement(T value) : value(value) {}

/**
 * Returns value of the node.
 *
 * @tparam T Type of the data held by this element.
 * @return Value of the node.
 */
template<class T>
T ListElement<T>::getValue() const {
    return value;
}

/**
 * Sets value of the node to the given one.
 *
 * @tparam T Type of the data held by this element.
 * @param value Value to set.
 */
template<class T>
void ListElement<T>::setValue(T value) {
    ListElement<T>::value = value;
}

/**
 * Returns element that comes before this node in the list.
 *
 * @tparam T Type of the data held by this element.
 * @return Pointer to the previous node.
 */
template<class T>
ListElement<T> *ListElement<T>::getPrev() const {
    return prev;
}

/**
 * Sets previous element of the list.
 *
 * @tparam T Type of the data held by this element.
 * @param prev Pointer to node to set as the previous element.
 */
template<class T>
void ListElement<T>::setPrev(ListElement<T> *prev) {
    ListElement<T>::prev = prev;
}

/**
 * Returns element that comes after this node in the list.
 *
 * @tparam T Type of the data held by this element.
 * @return Pointer to the next node.
 */
template<class T>
ListElement<T> *ListElement<T>::getNext() const {
    return next;
}

/**
 * Sets next element of the list.
 *
 * @tparam T Type of the data held by this element.
 * @param next Pointer to node to set as the next element.
 */
template<class T>
void ListElement<T>::setNext(ListElement<T> *next) {
    ListElement<T>::next = next;
}
