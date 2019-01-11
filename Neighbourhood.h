//
// Created by bazyli on 11.01.19.
//

#ifndef TSP_NEIHBOURHOOD_H
#define TSP_NEIHBOURHOOD_H

/**
 * Types of path neighbourhood.
 */
enum class Neighbourhood {

    /** Swap cities x and y. */
    SWAP,

    /** Insert city x at the place of y and shift others. */
    INSERT,

    /** Invert city order between x and y. */
    INVERT

};


#endif //TSP_NEIHBOURHOOD_H
