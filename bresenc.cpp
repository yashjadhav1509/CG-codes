//bresen circle
#include <GL/glut.h>

constexpr float ONE = 1.0f;
constexpr float ZERO = 0.0f;

void plot_point(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void plot_circle_points(int x0, int y0, int x, int y) {
    plot_point(x0 + x, y0 + y);
    plot_point(x0 - x, y0 + y);
    plot_point(x0 + x, y0 - y);
    plot_point(x0 - x, y0 - y);
    plot_point(x0 + y, y0 + x);
    plot_point(x0 - y, y0 + x);
    plot_point(x0 + y, y0 - x);
    plot_point(x0 - y, y0 - x);
}

void bresenham_circle(int x0, int y0, int radius) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    plot_circle_points(x0, y0, x, y);

    while (x <= y) {
        if (d < 0) {
            d += 4 * x + 6;
        } else {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
        plot_circle_points(x0, y0, x, y);
    }
}

void display() {
    int radius = 100;  // Define the radius here
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(ONE, ONE, ONE);

    bresenham_circle(300, 300, radius);

    glFlush();
}

void init() {
    glClearColor(ZERO, ZERO, ZERO, ZERO);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 700, 0, 700);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Bresenham Circle Drawing");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
