//dda line


#include <GL/glut.h>
#include <cmath>
#include <iostream>
using namespace std;

#define zero 0.0
#define one 1.0

int a, b, c, d, type;
int cnt = 0;
int oldx, oldy;
int newx, newy;

// Forward declaration of the menu function
void menu(int selectedType);

// Color Function to assign colors based on the line type
void setColor(int type) {
    if (type == 1) {
        glColor3f(1.0, 0.0, 0.0); // Red for Simple Line
    } else if (type == 2) {
        glColor3f(0.0, 0.0, 1.0); // Blue for Dotted Line
    } else if (type == 3) {
        glColor3f(0.0, 1.0, 0.0); // Green for Dashed Line
    } else if (type == 4) {
        glColor3f(0.0, 0.0, 0.0); // Black for Solid Line
    }
}

// DDA Line Drawing Function
void dda(int x1, int y1, int x2, int y2, int type) {
    float step, x, y, k, Xin, Yin;
    int dx = x2 - x1;
    int dy = y2 - y1;

    if (abs(dx) > abs(dy))
        step = abs(dx);
    else
        step = abs(dy);

    Xin = dx / step;
    Yin = dy / step;

    x = x1;
    y = y1;

    setColor(type); // Set the color based on the line type

    if (type == 4)  // Solid line is black with larger point size
        glPointSize(5.0f); 
    else
        glPointSize(1.0f);

    glBegin(GL_POINTS);
    glVertex2i(round(x), round(y));
    
    int j = 0;
    for (k = 1; k <= step; k++) {
        x += Xin;
        y += Yin;

        if (type == 1 || type == 4) { // Simple or Solid Line
            glVertex2i(round(x), round(y));
        }
        else if (type == 2 && j % 4 == 0) { // Dotted Line (point every 4th pixel)
            glVertex2i(round(x), round(y));
        }
        else if (type == 3 && j % 10 < 5) { // Dashed Line (draw a point every 5 steps)
            glVertex2i(round(x), round(y));
        }

        j = (j + 1) % 10; // Increase the counter for dotted/dashed lines
    }
    glEnd();
    glFlush();
}

// Display Function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw X and Y axes
    dda(-350, 0, 350, 0, 1); // X-axis
    dda(0, 350, 0, -350, 1); // Y-axis

    glFlush();
}

// Mouse Function
void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        int winWidth = viewport[2];
        int winHeight = viewport[3];

        int xi = x - winWidth / 2;
        int yi = winHeight / 2 - y;

        cnt = (cnt + 1) % 2;

        if (cnt == 1) { // first point
            oldx = xi;
            oldy = yi;
        } else { // second point
            newx = xi;
            newy = yi;

            // Now trigger the line drawing function when both points are clicked
            menu(type); // Draw line based on the selected type
        }

        glPointSize(5.0f);
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_POINTS);
        glVertex2i(xi, yi);
        glEnd();
        glFlush();
    }
}

// Menu Handler
void menu(int selectedType) {
    if (cnt == 0) { // Only after two clicks (start and end points)
        dda(oldx, oldy, newx, newy, selectedType);
    }
    type = selectedType; // Update the line type based on the menu selection
}

// Initialization
void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0); // White background
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-350, 350, -350, 350); // Set origin (0,0) at center
}

// Main Function
int main(int argc, char** argv) {
    a = 200, b = -200, c = -200, d = 200;
    type = 1;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("DDA Line Drawing with Mouse and Menu");

    init();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);

    // Create Right-click Menu
    glutCreateMenu(menu);
    glutAddMenuEntry("DDA Simple Line", 1);
    glutAddMenuEntry("DDA Dotted Line", 2);
    glutAddMenuEntry("DDA Dashed Line", 3);
    glutAddMenuEntry("DDA Solid Line", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    // Initial prompt for line type selection
    cout << "Select Line Type (1 for Simple, 2 for Dotted, 3 for Dashed, 4 for Solid): ";
    cin >> type;

    glutMainLoop();
    return 0;
}
