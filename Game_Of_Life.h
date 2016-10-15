#ifndef GAME_OF_LIFE_2_1_GAME_OF_LIFE_H
#define GAME_OF_LIFE_2_1_GAME_OF_LIFE_H

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include "Coordinate.h"

using namespace std;

class Game_Of_Life {
private:
    // constants
    static const size_t WIDTH = 512, HEIGHT = 384;

    // instances
    int number_of_species;
    Species grid[HEIGHT][WIDTH];
    vector<Coordinate> update_list;

    // private methods
    void random_spawn_grid();
    size_t number_of_neighbors(int x, int y, Species s);

public:
    Game_Of_Life(int num_species);
    void spawn_cell(int x, int y, Species s);
    void kill_cell(int x, int y);
    Species species_at_cell(int x, int y);
    void generate_update_list();
    vector<Coordinate> get_update_list();
    size_t get_width();
    size_t get_height();

};


#endif //GAME_OF_LIFE_2_1_GAME_OF_LIFE_H
