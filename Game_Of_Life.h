#ifndef GAME_OF_LIFE_2_1_GAME_OF_LIFE_H
#define GAME_OF_LIFE_2_1_GAME_OF_LIFE_H

#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <tbb/tbb.h>
#include <vector>

#include "Coordinate.h"

using namespace tbb;

class Game_Of_Life {
private:
    // constants
    static const size_t WIDTH = 512, HEIGHT = 384;

    // instances
    size_t number_of_species;
    Species grid[HEIGHT][WIDTH];
    concurrent_vector<Coordinate> update_list;

    // private methods
    void random_spawn_grid();
    size_t number_of_neighbors(size_t x, size_t y, Species s);
    void generate_update_list();
    void sequential_generate_update_list();

public:
    // constructor
    Game_Of_Life(size_t num_species);

    // public methods
    void set_cell(size_t x, size_t y, Species s);
    Species species_at_cell(size_t x, size_t y);
    concurrent_vector<Coordinate> get_update_list();
    size_t get_width();
    size_t get_height();

};

#endif
