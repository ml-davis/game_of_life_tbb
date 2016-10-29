#ifndef GAME_OF_LIFE_2_1_COORDINATE_H
#define GAME_OF_LIFE_2_1_COORDINATE_H

#include "Game_Of_Life.h"
#include <iostream>

using namespace std;

enum Species { S0, S1, S2, S3, S4, S5, S6, S7, S8, S9, DEAD };

struct Coordinate {
    size_t x;
    size_t y;
    Species species;
    Coordinate(size_t x_coordinate, size_t y_coordinate, Species s) {
        x = x_coordinate;
        y = y_coordinate;
        species = s;
    }
};

struct Counter {
    Species species;
    size_t count;
    Counter() {
        species = DEAD;
        count = 0;
    }
    Counter(Species s, size_t c) {
        species = s;
        count = c;
    }
};

// print error message and terminate program
inline void invalid_range(size_t x, string func) {
    cout << "Invalid range (" << x << ") in " << func << endl;
    exit(1);
}

// print error message and terminate program
inline void invalid_range(size_t x, size_t y, string func) {
    cout << "Invalid range (" << x << ", " << y << ") in " << func << endl;
    exit(1);
}

#endif
