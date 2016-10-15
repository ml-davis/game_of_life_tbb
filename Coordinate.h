#ifndef GAME_OF_LIFE_2_1_COORDINATE_H
#define GAME_OF_LIFE_2_1_COORDINATE_H

#include "Game_Of_Life.h"

enum Species { S0, S1, S2, S3, S4, S5, S6, S7, S8, S9, DEAD };

struct Coordinate {
    int x_coordinate;
    int y_coordinate;
    Species species;
    Coordinate(int x, int y, Species s) {
        x_coordinate = x;
        y_coordinate = y;
        species = s;
    }
};

#endif //GAME_OF_LIFE_2_1_COORDINATE_H
