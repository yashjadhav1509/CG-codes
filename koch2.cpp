//koch curve
#include<iostream>
#include<GL/glut.h>
using namespace std;

float startX, startY, endX, endY;
int n;

void getdata() {
    cout << "Enter start (x1 y1) and end (x2 y2) points of line: ";
    cin >> startX >> startY >> endX >> endY;
    cout << "Enter number of iterations: ";
    cin >> n;
}


void koch(float x1, float y1, float x2, float y2, int n) {
    if (n == 0) {
        glBegin(GL_LINES);
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
        glEnd();
        return;
    }

    float deltaX = (x2 - x1) / 3;
    float deltaY = (y2 - y1) / 3;

    float x3 = x1 + deltaX;
    float y3 = y1 + deltaY;

    float x4 = x1 + 2 * deltaX;
    float y4 = y1 + 2 * deltaY;

    float angle = 60.0 * 3.14159265 / 180.0;
    float peakX = x3 + (x4 - x3) * 0.5 - (y4 - y3) * 0.866;
    float peakY = y3 + (y4 - y3) * 0.5 + (x4 - x3) * 0.866;

    if (n == 1) {
        glBegin(GL_LINE_STRIP);
            glVertex2f(x1, y1);
            glVertex2f(x3, y3);
            glVertex2f(peakX, peakY);
            glVertex2f(x4, y4);
            glVertex2f(x2, y2);
        glEnd();
    }
    else {
        koch(x1, y1, x3, y3, n-1);
        koch(x3, y3, peakX, peakY, n-1);
        koch(peakX, peakY, x4, y4, n-1);
        koch(x4, y4, x2, y2, n-1);
    }
}

void Init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);  // Black background
    glColor3f(0.0, 1.0, 1.0);          // Cyan lines
    gluOrtho2D(0.0, 640.0, 0.0, 480.0); // Coordinate system (x:0-640, y:0-480)
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Center the figure
    float midX = (startX + endX) / 2;
    float midY = (startY + endY) / 2;
    float shiftX = 320 - midX;
    float shiftY = 240 - midY;

    koch(startX + shiftX, startY + shiftY, endX + shiftX, endY + shiftY, n);

    glFlush();
}

int main(int argc, char** argv) {
    getdata();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Koch Curve");
    Init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
