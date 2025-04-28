#include <iostream>
#include <GL/glut.h>
#include <cmath>
#include <ctime>

using namespace std;

void delay(float ms) {
    clock_t goal = ms + clock();
    while(goal > clock());
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);  // White background
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 640, 0, 480);        // 2D orthographic projection
}

// Function to compare colors with a small tolerance to avoid floating-point precision issues
bool compareColor(float* color1, float* color2) {
    const float tolerance = 0.1f;
    for (int i = 0; i < 3; i++) {
        if (abs(color1[i] - color2[i]) > tolerance) {
            return false;
        }
    }
    return true;
}

void bound_it(int x, int y, float* fillColor, float* bc) {
    float color[3];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, color);  // Get the color of the current pixel

    // If the current pixel is not the boundary color and is not filled already
    if (!compareColor(color, bc) && !compareColor(color, fillColor)) {
        glColor3f(fillColor[0], fillColor[1], fillColor[2]);
        glBegin(GL_POINTS);
        glVertex2i(x, y);
        glEnd();
        glFlush();

        // Call the recursive function to fill adjacent pixels
        bound_it(x + 1, y, fillColor, bc);  // Right
        bound_it(x - 1, y, fillColor, bc);  // Left
        bound_it(x, y + 1, fillColor, bc);  // Up
        bound_it(x, y - 1, fillColor, bc);  // Down
    }
}

void mouse(int btn, int state, int x, int y) {
    y = 480 - y; // Invert y-coordinate to match OpenGL's bottom-left origin
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float boundaryColor[] = {1, 0, 0}; // Red boundary
        float fillColor[] = {0, 0, 1};     // Blue fill color
        bound_it(x, y, fillColor, boundaryColor);
    }
}

void world() {
    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(3);
    glPointSize(2);
    glColor3f(1, 0, 0);  // Red color for boundary
    glBegin(GL_LINE_LOOP);
        glVertex2i(150, 100);
        glVertex2i(300, 300);
        glVertex2i(450, 100);
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Boundary Fill Algorithm");

    glutDisplayFunc(world);
    glutMouseFunc(mouse);
    init();
    glutMainLoop();
    return 0;
}