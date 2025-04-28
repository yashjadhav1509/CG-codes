//line bresen

#include <GL/glut.h>
#include <iostream>
#include <cmath>

#define zero 0.0
#define one 1.0

using namespace std;

int a, b, c, d, type;
int oldx, oldy, newx, newy, cnt = 0;
int lineType = 1;  // Default line type: Simple Line
float thickness = 1.0f;

void drawpixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void setLineColor(int type) {
    // Set color based on line type
    if (type == 1) {
        glColor3f(1.0, 1.0, 1.0); // White for Simple Line
    } else if (type == 2) {
        glColor3f(1.0, 0.0, 0.0); // Red for Dotted Line
    } else if (type == 3) {
        glColor3f(0.0, 0.0, 1.0); // Blue for Dashed Line
    } else if (type == 4) {
        glColor3f(0.0, 1.0, 0.0); // Green for Solid Line
    }
}

void BresenhamLine(int x1, int y1, int x2, int y2, int type) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int incx = (dx >= 0) ? 1 : -1;
    int incy = (dy >= 0) ? 1 : -1;
    dx = abs(dx);
    dy = abs(dy);
    int x = x1, y = y1;
    int e;
    
    // Set line color based on the type
    setLineColor(type);

    if (dx > dy) {
        e = 2 * dy - dx;
        int j = 0;
        for (int i = 0; i <= dx; i++) {
            if (e >= 0) {
                y += incy;
                e += 2 * (dy - dx);
            } else {
                e += 2 * dy;
            }
            x += incx;

            // Draw based on the line type
            if (type == 1 || type == 4) {
                drawpixel(x, y);  // Simple or Solid Line
            }

            // Dotted Line
            if (type == 2 && j % 2 == 0) {
                drawpixel(x, y);
            }

            // Dashed Line
            if (type == 3 && j < 5) {
                drawpixel(x, y);
            }

            j = (j + 1) % 10;
        }
    } else {
        e = 2 * dx - dy;
        int j = 0;
        for (int i = 0; i <= dy; i++) {
            if (e >= 0) {
                x += incx;
                e += 2 * (dx - dy);
            } else {
                e += 2 * dx;
            }
            y += incy;

            // Draw based on the line type
            if (type == 1 || type == 4) {
                drawpixel(x, y);  // Simple or Solid Line
            }

            // Dotted Line
            if (type == 2 && j % 2 == 0) {
                drawpixel(x, y);
            }

            // Dashed Line
            if (type == 3 && j < 5) {
                drawpixel(x, y);
            }

            j = (j + 1) % 10;
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the coordinate axes
    BresenhamLine(-350, 0, 350, 0, 1);  // X-axis
    BresenhamLine(0, 350, 0, -350, 1);  // Y-axis

    glFlush();
}

void init() {
    glClearColor(zero, zero, zero, zero);
    gluOrtho2D(-350, 350, -350, 350);  // Set orthogonal view for all quadrants
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        int winWidth = viewport[2];
        int winHeight = viewport[3];

        int xi = x - winWidth / 2;
        int yi = winHeight / 2 - y;

        cnt = (cnt + 1) % 2;

        if (cnt == 1) {
            oldx = xi;
            oldy = yi;
        }

        if (cnt == 0) {
            newx = xi;
            newy = yi;

            glClear(GL_COLOR_BUFFER_BIT);
            // Draw the coordinate axes again
            glColor3f(1.0, 1.0, 1.0);
            BresenhamLine(-350, 0, 350, 0, 1);  // X-axis
            BresenhamLine(0, 350, 0, -350, 1);  // Y-axis

            // Draw the selected line type
            BresenhamLine(oldx, oldy, newx, newy, lineType);
            glFlush();
        }
    }
}

void menu(int option) {
    if (option == 9) exit(0);

    lineType = option;
    glClear(GL_COLOR_BUFFER_BIT);
    // Draw the coordinate axes again
    glColor3f(1.0, 1.1, 1.0);
    BresenhamLine(-350, 0, 350, 0, 1);  // X-axis
    BresenhamLine(0, 350, 0, -350, 1);  // Y-axis

    // Draw the selected line
    BresenhamLine(oldx, oldy, newx, newy, lineType);
    glFlush();
}

void userInputForLineType() {
    cout << "Enter the line type (1: Simple, 2: Dotted, 3: Dashed, 4: Solid): ";
    cin >> lineType;
    cout << "Line type changed to: " << lineType << endl;
    // Clear and redraw the screen with the selected line type
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    BresenhamLine(-350, 0, 350, 0, 1);  // X-axis
    BresenhamLine(0, 350, 0, -350, 1);  // Y-axis
    BresenhamLine(oldx, oldy, newx, newy, lineType);
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Bresenham Line Drawing");

    // Create the menu for line types
    glutCreateMenu(menu);
    init();
    glutMouseFunc(mouse);
    glutAddMenuEntry("Simple Line", 1);
    glutAddMenuEntry("Dotted Line", 2);
    glutAddMenuEntry("Dashed Line", 3);
    glutAddMenuEntry("Solid Line", 4);
    glutAddMenuEntry("Exit", 9);
    glutDisplayFunc(display);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    cout << "Use the mouse to draw lines. Right-click to select the line type from the menu." << endl;

    // User input for changing the line type
    userInputForLineType();

    glutMainLoop();

    return 0;
}
