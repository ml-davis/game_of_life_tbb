// This file contains all of our game logic source code. It has the methods
// which determine which cells should live or die.

#include "Game_Of_Life.h"

// constructor
Game_Of_Life::Game_Of_Life(size_t num_species) {
    if (num_species < 1 || num_species > 10) {
        invalid_range(num_species, "constructor");
    }

    srand(time(NULL));

    number_of_species = num_species;

    // initialize empty board
    for (size_t i = 0; i < HEIGHT; i++) {
        for (size_t j = 0; j < WIDTH; j++) {
            set_cell(j, i, DEAD);
        }
    }

    random_spawn_grid();
}

// place all species randomly on board (buckshot approach)
void Game_Of_Life::random_spawn_grid() {
    for (size_t i = 0; i < number_of_species; i++) {
        Species species = static_cast<Species>(i);

        int radius = 45;
        int number_of_squares = (int) floor((radius * radius) * 0.20); // fill ~20% of square
        int distance_from_edge = radius + 2;

        // choose random target on board, at least specified distance from edges
        size_t x_target = (rand() % (WIDTH - (distance_from_edge * 2 - 1))) + distance_from_edge;
        size_t y_target = (rand() % (HEIGHT - (distance_from_edge * 2 - 1))) + distance_from_edge;

        set_cell(x_target, y_target, species);

        // pick number_of_squares within (radius x radius) square centered on target
        size_t rand_x;
        size_t rand_y;
        for (int i = 0; i < number_of_squares; i++) {
            rand_x = x_target + ((rand() % (radius + 1)) - (radius/2));
            rand_y = y_target + ((rand() % (radius + 1)) - (radius/2));
            set_cell(rand_x, rand_y, species);
        }
    }
}

// determine number of neighbors of given species at coordinate (x, y)
size_t Game_Of_Life::number_of_neighbors(size_t x, size_t y, Species s) {
    if (x < 0 || y < 0 || x >= WIDTH|| y >= HEIGHT) {
        invalid_range(x, y, "number_of_neighbors");
    }

    size_t count = 0;

    // iterate over 3x3 grid centered on (x,y)
    for (size_t i = x - 1; i <= x + 1; i++) {
        for (size_t j = y - 1; j <= y + 1; j++) {
            // check only if cell isn't current cell (x,y) AND cell is not out of bounds
            if ((i != x || j != y) && (i >= 0 && j >= 0 && i < WIDTH && j < HEIGHT)) {
                if (species_at_cell(i, j) == s) {
                    count++;
                }
            }
        }
    }

    return count;
}

// uses TBB to determine in parallel which cells on board need to change (be spawned or killed)
void Game_Of_Life::generate_update_list() {
    update_list.clear();

    parallel_for(blocked_range<size_t>(0, WIDTH * HEIGHT - 1),
        [=](const blocked_range<size_t> &r) {
            for (size_t i = r.begin(); i <= r.end(); i++) {

                size_t x = i % WIDTH;
                size_t y = i / WIDTH ;

                // determine species of cell
                Species species = species_at_cell(x, y);

                size_t num_neighbors;

                // if species lives in cell, count # neighbors and add to kill list if applicable
                if (species != DEAD) {
                    num_neighbors = number_of_neighbors(x, y, species);
                    if (num_neighbors < 2 || num_neighbors > 3) {
                        update_list.push_back(Coordinate(x, y, DEAD));
                    }
                    // if no species in cell, check if any species should be spawned there
                } else {
                    for (size_t k = 0; k < number_of_species; k++) {
                        species = static_cast<Species>(k);
                        num_neighbors = number_of_neighbors(x, y, species);
                        if (num_neighbors == 3) {
                            update_list.push_back(Coordinate(x, y, species));
                            break;
                        }
                    }
                }
            }
        }, auto_partitioner());
}

// determine which cells on board need to change (be spawned or killed) can be removed later
void Game_Of_Life::sequential_generate_update_list() {
    update_list.clear();

    // for each cell
    for (size_t i = 0; i < HEIGHT; i++) {
        for (size_t j = 0; j < WIDTH; j++) {

            // determine species of cell
            Species species = species_at_cell(j, i);
            size_t num_neighbors;

            // if species lives in cell, count # neighbors and add to kill list if applicable
            if (species != DEAD) {
                num_neighbors = number_of_neighbors(j, i, species);
                if (num_neighbors < 2 || num_neighbors > 3) {
                    update_list.push_back(Coordinate(j, i, DEAD));
                }
            // if no species in cell, check if any species should be spawned there
            } else {
                for (size_t k = 0; k < number_of_species; k++) {
                    species = static_cast<Species>(k);
                    num_neighbors = number_of_neighbors(j, i, species);
                    if (num_neighbors == 3) {
                        update_list.push_back(Coordinate(j, i, species));
                        break;
                    }
                }
            }
        }
    }
}

// set species of given cell
void Game_Of_Life::set_cell(size_t x, size_t y, Species s) {
    if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT) {
        invalid_range(x, y, "set_cell");
    }
    grid[y][x] = s;
}

// return the species at current cell
Species Game_Of_Life::species_at_cell(size_t x, size_t y) {
    if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT) {
        invalid_range(x, y, "species_at_cell");
    }
    return grid[y][x];
}

// first refresh the update list and then return it
concurrent_vector<Coordinate> Game_Of_Life::get_update_list() {
//    sequential_generate_update_list();
    generate_update_list();
    return update_list;
}

// returns the width of board
size_t Game_Of_Life::get_width() {
    return WIDTH;
}

// returns the height of board
size_t Game_Of_Life::get_height() {
    return HEIGHT;
}
