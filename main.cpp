// This file calls our main function. It contains all OpenGL source code. It has an object of
// Game_Of_Life which it uses to determine the color of squares.

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Game_Of_Life.h"

Game_Of_Life game(5);

int pixel_width = 1024;
int pixel_height = 768;

size_t cell_size = pixel_width / game.get_width();

// sets the color that OpenGL will draw with
void set_color(Species species) {
    switch(species) {
        case S0:	glColor3f(1.0f, 0.0f, 0.0f); break; // RED
        case S1:	glColor3f(0.0f, 1.0f, 0.0f); break; // GREEN
        case S2:	glColor3f(0.1f, 0.2f, 1.0f); break; // BLUE
        case S3:	glColor3f(1.0f, 1.0f, 0.0f); break; // YELLOW
        case S4:	glColor3f(1.0f, 0.0f, 1.0f); break; // MAGENTA
        case S5:	glColor3f(0.0f, 1.0f, 1.0f); break; // CYAN
        case S6:	glColor3f(1.0f, 1.0f, 1.0f); break; // WHITE
        case S7:	glColor3f(1.0f, 0.5f, 0.0f); break; // ORANGE
        case S8:	glColor3f(0.5f, 0.5f, 0.5f); break; // GREY
        case S9:	glColor3f(0.4f, 0.0f, 1.0f); break; // VIOLET
        default:	glColor3f(0.0f, 0.0f, 0.0f);		// BLACK
    }
}

// places a square at (x, y). Must be nested in glBegin() <-> glEnd() tags
void draw_square(int x, int y) {
    x *= cell_size;
    y *= cell_size;

    glVertex2f(x, y);
    glVertex2f(x + cell_size, y);
    glVertex2f(x + cell_size, y + cell_size);
    glVertex2f(x, y + cell_size);
}

// iterates over entire board and fills it in according to Game_Of_Life object
void initialize_board() {
    glBegin(GL_QUADS);
    for (size_t i = 0; i < game.get_height(); i++) {
        for (size_t j = 0; j < game.get_width(); j++) {
            Species species = game.species_at_cell(j, i);
            set_color(species);
            draw_square(j, i);
        }
    }
    glEnd();
    glFlush();
}

// infinite loop. It fetches and operates on the cells that need to be changed forever
void display() {
    initialize_board();
    for (;;) {
        glBegin(GL_QUADS);

        vector<Coordinate> update_list = game.get_update_list(); // renewed every fetch

        for (auto &coord: update_list) {
            set_color(coord.species);
            game.set_cell(coord.x, coord.y, coord.species);
            draw_square(coord.x, coord.y);
        }

        glEnd();
        glFlush();
    }
}

// intialization
int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(pixel_width, pixel_height);
    glutCreateWindow("Game of Life 2.1");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, pixel_width, pixel_height, 0.0f, -1.0f, 1.0f);

    glutDisplayFunc(display);

    glutMainLoop();
}