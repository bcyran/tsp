//
// Created by bazyli on 11.10.18.
//

#ifndef C_PATH_H
#define C_PATH_H

#include <string>

using namespace std;

class Path {

    /** Length of the path. */
    int length = 0;

    /** Path as array of cities. */
    int *path = nullptr;

    /** Distance of the path. */
    int distance = -1;

public:

    Path();

    explicit Path(int length);

    Path(int length, int *path);

    Path(const Path &src);

    Path &operator=(const Path &src);

    virtual ~Path();

    bool inPath(int city, int limit);

    bool permute(int start, int end);

    int getLength() const;

    int getPoint(int index);

    void setPoint(int index, int city);

    int *getPath() const;

    int getDistance() const;

    void setDistance(int distance);

    string toString();

};


#endif //C_PATH_H
