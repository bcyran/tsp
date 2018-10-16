//
// Created by Bazyli on 05.03.18.
//

#ifndef PROJEKT1_LISTELEMENT_H
#define PROJEKT1_LISTELEMENT_H

/**
 * Single element of the list.
 *
 * @tparam T Type of the data held by this element.
 */
template<class T>
class ListElement {

    /** Value stored in this element. */
    T value;

    /** Pointer to the previous element. */
    ListElement *prev = nullptr;

    /** Pointer to the next element. */
    ListElement *next = nullptr;

public:

    ListElement();

    explicit ListElement(T value);

    T getValue() const;

    void setValue(T value);

    ListElement<T> *getPrev() const;

    void setPrev(ListElement<T> *prev);

    ListElement<T> *getNext() const;

    void setNext(ListElement<T> *next);
};


#include "ListElement.tcc"

#endif //PROJEKT1_LISTELEMENT_H
