// kochsnow
#include <GL/glut.h>
#include <iostream>
#include <cmath>
using namespace std;

// Start and end points
float x_start, y_start, x_end, y_end;
int depth;

// Function to draw Koch curve
void draw_koch(float xa, float ya, float xb, float yb, int n) {
    if (n == 0) {
        glVertex2f(xa, ya);
        glVertex2f(xb, yb);
    } else {
        float dx = (xb - xa) / 3;
        float dy = (yb - ya) / 3;

        float x1 = xa + dx;
        float y1 = ya + dy;

        float x2 = xa + 2 * dx;
        float y2 = ya + 2 * dy;

        float mx = (x1 + x2) / 2 - (sqrt(3) * (y2 - y1)) / 2;
        float my = (y1 + y2) / 2 + (sqrt(3) * (x2 - x1)) / 2;

        draw_koch(xa, ya, x1, y1, n - 1);
        draw_koch(x1, y1, mx, my, n - 1);
        draw_koch(mx, my, x2, y2, n - 1);
        draw_koch(x2, y2, xb, yb, n - 1);
    }
}

// Function to draw snowflake (3 Koch curves)
void draw_snowflake(float xc, float yc, float size, int depth) {
    // Calculate the three corners of an equilateral triangle
    float angle = M_PI / 3.0;  // 60 degrees in radians
    float x1 = xc - size / 2;
    float y1 = yc - size / sqrt(3); // The height of the equilateral triangle
    float x2 = xc + size / 2;
    float y2 = y1;
    float x3 = xc;
    float y3 = yc + size / sqrt(3);

    // Draw three Koch curves forming a snowflake
    glBegin(GL_LINES);
    draw_koch(x1, y1, x2, y2, depth); // First side
    draw_koch(x2, y2, x3, y3, depth); // Second side
    draw_koch(x3, y3, x1, y1, depth); // Third side
    glEnd();
}

// Display function to render the snowflake
void display() {
    glClear(GL_COLOR_BUFFER_BIT);  // Clear previous frame
    glColor3f(0, 0, 0); // Set drawing color to black

    // Call the function to draw the snowflake
    draw_snowflake(250, 250, 200, depth); // Center at (250,250) with size 200

    glFlush(); // Finish drawing and display the result
}

// Initialization function
void init() {
    glClearColor(1, 1, 1, 1); // Set background to white
    glColor3f(0, 0, 0); // Set color to black
    glMatrixMode(GL_PROJECTION); 
    gluOrtho2D(0, 500, 0, 500); // Set orthographic view to fit within 500x500 window
}

// Function to get user input
void getdata() {
    cout << "Enter depth of recursion: ";
    cin >> depth;
}

int main(int argc, char** argv) {
    getdata(); // Get user input for depth of recursion

    glutInit(&argc, argv); // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Set display mode to single and RGB color
    glutInitWindowSize(500, 500); // Set window size to 500x500
    glutInitWindowPosition(100, 100); // Position the window
    glutCreateWindow("Koch Snowflake"); // Create the window

    init(); // Initialize OpenGL settings
    glutDisplayFunc(display); // Register the display function
    glutMainLoop(); // Start the GLUT main loop

    return 0; // Exit the program
}
