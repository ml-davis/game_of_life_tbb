#include "Game_Of_Life.h"

// print error message and terminate program
void invalid_range(int x, int y, string func) {
    cout << "Invalid range (" << x << ", " << y << ") in " << func
              << endl;
    exit(1);
}

Game_Of_Life::Game_Of_Life(int num_species) {
    srand(time(NULL));

    number_of_species = num_species;

    // initialize dead board
    for (size_t i = 0; i < HEIGHT; i++) {
        for (size_t j = 0; j < WIDTH; j++) {
            kill_cell(j, i);
        }
    }

    random_spawn_grid();
}

void Game_Of_Life::random_spawn_grid() {
    for (size_t i = 0; i < number_of_species; i++) {
        Species species = static_cast<Species>(i);

        int distance_from_edge = 12;
        int number_of_squares = 60;
        int radius = 10; // DANGER: do not set radius >= distance_from_edge or index out of bounds

        // choose random target on board, at least specified distance from edges
        int x_target = (rand() % (WIDTH - (distance_from_edge * 2 - 1))) + distance_from_edge;
        int y_target = (rand() % (HEIGHT - (distance_from_edge * 2 - 1))) + distance_from_edge;

        spawn_cell(x_target, y_target, species);

        // pick number_of_squares within radius x radius square centered on target
        int rand_x;
        int rand_y;
        for (int i = 0; i < number_of_squares; i++) {
            rand_x = x_target + ((rand() % (radius + 1)) - (radius/2));
            rand_y = y_target + ((rand() % (radius + 1)) - (radius/2));
            spawn_cell(rand_x, rand_y, species);
        }
    }
}

size_t Game_Of_Life::number_of_neighbors(int x, int y, Species s) {
    if (x < 0 || y < 0 || x >= WIDTH|| y >= HEIGHT) {
        invalid_range(x, y, "number_of_neighbors");
    }

    size_t count = 0;

    // iterate over 3x3 grid centered on (x,y)
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
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

void Game_Of_Life::spawn_cell(int x, int y, Species s) {
    grid[y][x] = s;
}

void Game_Of_Life::kill_cell(int x, int y) {
    grid[y][x] = DEAD;
}

Species Game_Of_Life::species_at_cell(int x, int y) {
    return grid[y][x];
}

void Game_Of_Life::generate_update_list() {
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

vector<Coordinate> Game_Of_Life::get_update_list() {
    return update_list;
}

size_t Game_Of_Life::get_width() {
    return WIDTH;
}

size_t Game_Of_Life::get_height() {
    return HEIGHT;
}
