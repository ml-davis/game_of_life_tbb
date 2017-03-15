// This file contains all of our game logic source code. It has the methods
// which determine which cells should live or die.

#include "Game_Of_Life.h"

// constructor
Game_Of_Life::Game_Of_Life(size_t num_species) {
    if (num_species < 1 || num_species > 10) {
        cout << "Invalid range in constructor. Number of species must be [1-10]" << endl;
        exit(1);
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

// place all species randomly on board ("buckshot" approach)
void Game_Of_Life::random_spawn_grid() {

// 	// Completely random spawn type
//	for (size_t i = 0; i < number_of_species; i++) {
//		for (size_t j = 0; j < HEIGHT; j++) {
//			for (size_t k = 0; k < WIDTH; k++) {
//				Species species = static_cast<Species>(rand() % 10);
//				set_cell(k, j, species); 
//			}
//		}
//	}

    for (size_t i = 0; i < number_of_species; i++) {
		for (size_t j = 0; j < 4; j++) {
			Species species = static_cast<Species>(i);

			size_t square_size = WIDTH * .10;
			// fill ~20% of square
			size_t number_of_squares = (size_t) floor((square_size * square_size) * 0.20);
			size_t distance_from_edge = square_size + 2;

			// choose random target on board, at least specified distance from edges
			size_t x_target = (rand() % (WIDTH - (distance_from_edge * 2 - 1))) + distance_from_edge;
			size_t y_target = (rand() % (HEIGHT - (distance_from_edge * 2 - 1))) + distance_from_edge;

			set_cell(x_target, y_target, species);

			// pick number_of_squares within (square_size x square_size) square centered on target
			size_t rand_x;
			size_t rand_y;
			for (size_t i = 0; i < number_of_squares; i++) {
				rand_x = x_target + ((rand() % (square_size + 1)) - (square_size/2));
				rand_y = y_target + ((rand() % (square_size + 1)) - (square_size/2));
				set_cell(rand_x, rand_y, species);
			}
		}
    }
}

// determine number of neighbors of given species at coordinate (x, y)
size_t Game_Of_Life::number_of_neighbors(size_t x, size_t y, Species s) {
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

bool Game_Of_Life::has_three_neighbors(size_t x, size_t y) {
    int count = 0;

    for (size_t i = x - 1; i <= x + 1; i++) {
        for (size_t j = y - 1; j <= y + 1; j++) {
            // check only if cell isn't current cell (x,y) AND cell is not out of bounds
            if ((i != x || j != y) && (i >= 0 && j >= 0 && i < WIDTH && j < HEIGHT)) {
                if (species_at_cell(i, j) != DEAD) {
                    if (++count == 3) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

// returns which type of cell should be spawned at (x, y) if any at all
Species Game_Of_Life::get_spawn_type(size_t x, size_t y) {

    // initialize counter
    Counter c[10];
    for (size_t i = 0; i < number_of_species; i++) {
        c[i] = Counter(static_cast<Species>(i), 0);
    }

    // get number of neighbors for each species
    for (size_t i = x - 1; i <= x + 1; i++) {
        for (size_t j = y - 1; j <= y + 1; j++) {
            // check only if cell isn't current cell (x,y) AND cell is not out of bounds
            if ((i != x || j != y) && (i >= 0 && j >= 0 && i < WIDTH && j < HEIGHT)) {
                Species s = species_at_cell(i, j);
                c[s].count++;
            }
        }
    }

    // if any species has 3 neighbors it will be added to update_list
    for (size_t i = 0; i < number_of_species; i++) {
        if (c[i].count == 3) {
            return c[i].species;
        }
    }

    // if no spawn cell found, cell should be dead
    return DEAD;
}

// uses TBB to determine in parallel which cells on board need to change (be spawned or killed)
void Game_Of_Life::generate_update_list() {
    update_list.clear();

    parallel_for(size_t(0), WIDTH * HEIGHT, [=](size_t i) {

        size_t x = i % WIDTH;
        size_t y = i / WIDTH ;

        // determine species of cell
        Species species = species_at_cell(x, y);

        // if species lives in cell, count # neighbors and add to kill list if applicable
        if (species != DEAD) {
            size_t num_neighbors = number_of_neighbors(x, y, species);
            if (num_neighbors < 2 || num_neighbors > 3) {
                update_list.push_back(Coordinate(x, y, DEAD));
            }
        // if no species in cell, check if any species should be spawned there
        // do "heavier" get_spawn_type() only if 3 neighbors exist
        } else if (has_three_neighbors(x, y)) {
            Species spawn_type = get_spawn_type(x, y);
            if (spawn_type != DEAD) {
                update_list.push_back(Coordinate(x, y, spawn_type));
            }
        }
    }, auto_partitioner());
}

// set species of given cell
void Game_Of_Life::set_cell(size_t x, size_t y, Species s) {
    grid[y][x] = s;
}

// return the species at current cell
Species Game_Of_Life::species_at_cell(size_t x, size_t y) {
    return grid[y][x];
}

// first refresh the update list and then return it
concurrent_vector<Coordinate> Game_Of_Life::get_update_list() {
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
