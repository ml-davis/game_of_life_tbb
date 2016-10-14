#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

int pixel_width = 1024;
int pixel_height = 768;

int cell_size = 8;

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

// colors a square at coordinates x,y a given color
void paint_cell(float x, float y, char species) {
    if (x < 0 || x >= horiz_num_cells || y < 0 || y >= vert_num_cells) {
        std::cout << "Tried to place cell in invalid position: (" << x << ", " << y << ")" << std::endl;
        exit(1);
    }

    x *= cell_size;
    y *= cell_size;

    set_color(species);

    glBegin(GL_POLYGON);
    glVertex3f(x, y, 0.0f);
    glVertex3f(x + cell_size, y, 0.0f);
    glVertex3f(x + cell_size, y + cell_size, 0.0f);
    glVertex3f(x, y + cell_size, 0.0f);
    glEnd();
    glFlush();
}


void display() {
    char species = 'A';
    int count = -1;
    for (int i = 0; i < vert_num_cells; i++) {
        for (int j = 0; j < horiz_num_cells; j++) {
            if (count++ == 9) {
                count = 0;
                species = 'A';
            }
            paint_cell(j, i, species++);
        }
    }
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