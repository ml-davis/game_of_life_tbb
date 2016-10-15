#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

int pixel_width = 1024;
int pixel_height = 768;

int cell_size = 4;

int horiz_num_cells = pixel_width / cell_size;
int vert_num_cells = pixel_height / cell_size;

void set_color(char species) {
    switch(species) {
        case 'A':	glColor3f(1.0f, 0.0f, 0.0f); break; // RED
        case 'B':	glColor3f(0.0f, 1.0f, 0.0f); break; // GREEN
        case 'C':	glColor3f(0.1f, 0.2f, 1.0f); break; // BLUE
        case 'D':	glColor3f(1.0f, 1.0f, 0.0f); break; // YELLOW
        case 'E':	glColor3f(1.0f, 0.0f, 1.0f); break; // MAGENTA
        case 'F':	glColor3f(0.0f, 1.0f, 1.0f); break; // CYAN
        case 'G':	glColor3f(1.0f, 1.0f, 1.0f); break; // WHITE
        case 'H':	glColor3f(1.0f, 0.5f, 0.0f); break; // ORANGE
        case 'I':	glColor3f(0.5f, 0.5f, 0.5f); break; // GREY
        case 'J':	glColor3f(0.4f, 0.0f, 1.0f); break; // VIOLET
        default:	glColor3f(0.0f, 0.0f, 0.0f);		// BLACK
    }
}

void display() {
    char species = 'A';
    int count = -1;

    glBegin(GL_QUADS);
    for (int i = 0; i < vert_num_cells; i++) {
        for (int j = 0; j < horiz_num_cells; j++) {
            if (count++ == 9) {
                count = 0;
                species = 'A';
            }

            set_color(species++);

            int x = j * cell_size;
            int y = i * cell_size;

            glVertex2f(x, y);
            glVertex2f(x + cell_size, y);
            glVertex2f(x + cell_size, y + cell_size);
            glVertex2f(x, y + cell_size);

        }
    }
    glEnd();
    glFlush();
}

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