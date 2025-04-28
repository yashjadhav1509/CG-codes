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







//cohen
#include <GL/glut.h>
#include <vector>

#define LEFT 1
#define RIGHT 2
#define BOTTOM 4
#define TOP 8

struct Point { float x, y; };
std::vector<Point> polygon, clippedPolygon;

float xmin = 100, ymin = 100, xmax = 400, ymax = 400;
bool clipFlag = false;

int getCode(float x, float y) {
    int code = 0;
    if (x < xmin) code |= LEFT;
    if (x > xmax) code |= RIGHT;
    if (y < ymin) code |= BOTTOM;
    if (y > ymax) code |= TOP;
    return code;
}

bool cohenSutherlandClip(Point &p1, Point &p2) {
    int code1 = getCode(p1.x, p1.y), code2 = getCode(p2.x, p2.y);
    while (true) {
        if (!(code1 | code2)) return true;
        if (code1 & code2) return false;
        int outcode = code1 ? code1 : code2;
        float x, y;

        if (outcode & TOP) {
            x = p1.x + (p2.x - p1.x) * (ymax - p1.y) / (p2.y - p1.y);
            y = ymax;
        } else if (outcode & BOTTOM) {
            x = p1.x + (p2.x - p1.x) * (ymin - p1.y) / (p2.y - p1.y);
            y = ymin;
        } else if (outcode & RIGHT) {
            y = p1.y + (p2.y - p1.y) * (xmax - p1.x) / (p2.x - p1.x);
            x = xmax;
        } else {
            y = p1.y + (p2.y - p1.y) * (xmin - p1.x) / (p2.x - p1.x);
            x = xmin;
        }

        if (outcode == code1) { p1.x = x; p1.y = y; code1 = getCode(p1.x, p1.y); }
        else { p2.x = x; p2.y = y; code2 = getCode(p2.x, p2.y); }
    }
}

void clipPolygon() {
    clippedPolygon.clear();
    for (size_t i = 0; i < polygon.size(); i++) {
        Point p1 = polygon[i], p2 = polygon[(i + 1) % polygon.size()];
        if (cohenSutherlandClip(p1, p2)) {
            clippedPolygon.push_back(p1);
            clippedPolygon.push_back(p2);
        }
    }
}

void drawPolygon(const std::vector<Point> &poly, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_LINE_LOOP);
    for (const auto &p : poly) glVertex2f(p.x, p.y);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(1, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xmin, ymin); glVertex2f(xmax, ymin);
    glVertex2f(xmax, ymax); glVertex2f(xmin, ymax);
    glEnd();

    if (clipFlag) drawPolygon(clippedPolygon, 0, 1, 0);
    else drawPolygon(polygon, 0, 0, 1);
    
    glFlush();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        polygon.push_back({(float)x, 500 - (float)y});
        glutPostRedisplay();
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        glutPostRedisplay();
    }
}

void menu(int option) {
    switch (option) {
        case 1:
            clipFlag = true;
            clipPolygon();
            glutPostRedisplay();
            break;
        case 2:
            clipFlag = false;
            glutPostRedisplay();
            break;
        case 3:
            exit(0);
            break;
    }
}

void createMenu() {
    int menu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Execute (Clip Polygon)", 1);
    glutAddMenuEntry("Show Clipped Polygon", 2);
    glutAddMenuEntry("Exit", 3);
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void keyboard(unsigned char key, int, int) {
    if (key == 'c') { clipFlag = true; clipPolygon(); glutPostRedisplay(); }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Cohen-Sutherland Polygon Clipping");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    createMenu();

    glClearColor(1, 1, 1, 1);
    glutMainLoop();
}





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






//animation
#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <unistd.h> // For usleep()
#include <ctime>    // For real-time clock

using namespace std;

const float g = 9.8f;  // Gravitational constant
const float L = 10.0f; // Length of the pendulum (scale factor)
float angle_max = 45.0f; // Maximum angular displacement (degrees)
float angle = angle_max; // Current angle of the pendulum

float time_elapsed = 0.0f; // Time elapsed for pendulum motion

void drawCircle(float segments, float radius, float sx, float sy) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(segments);
        float x = radius * cos(theta);
        float y = radius * sin(theta);
        glVertex2f(x + sx, y + sy);
    }
    glEnd();
}

void draw(float x1, float y1, float angle) {
    float segments = 100;
    float radius = 6.0f;

    // Main Clock Circle
    glColor3f(1, 0, 0);
    glLineWidth(4);
    drawCircle(segments, radius, x1, y1);

    // Get current time
    time_t t = time(0);
    struct tm *current_time = localtime(&t);
    int hour = current_time->tm_hour;
    int minute = current_time->tm_min;
    int second = current_time->tm_sec;

    // Adjust hour for 12-hour format (mod 12)
    hour = hour % 12;

    // Minute Hand (based on real time)
    float minuteAngle = (minute + second / 60.0f) * 6.0f; // Every minute is 6 degrees
    glColor3f(1, 1, 0);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x1 + radius * cos(minuteAngle * M_PI / 180.0f), y1 + radius * sin(minuteAngle * M_PI / 180.0f));
    glEnd();

    // Hour Hand (based on real time)
    float hourAngle = ((hour + minute / 60.0f) * 30.0f); // Every hour is 30 degrees
    glColor3f(1, 0, 0);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x1 + (radius / 1.5f) * cos(hourAngle * M_PI / 180.0f), y1 + (radius / 1.5f) * sin(hourAngle * M_PI / 180.0f));
    glEnd();

    // Pendulum (simulate smooth oscillation)
    // The pendulum angle follows the harmonic motion formula
    angle = angle_max * cos(sqrt(g / L) * time_elapsed);
    time_elapsed += 0.02f; // Update time for next frame

    // Calculate pendulum position
    float x2 = (L * 0.8f) * sin(angle * M_PI / 180.0f);
    float y2 = -(L * 0.8f) * cos(angle * M_PI / 180.0f);

    glColor3f(0, 0, 1);
    drawCircle(segments, 2.0f, x2 + x1, y2 + y1); // Pendulum bob

    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x1 + x2, y1 + y2); // Pendulum string
    glEnd();
}

void display() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -30.0f); // Center the clock in the viewport

    draw(0, 0, angle);

    glutSwapBuffers();
    glutPostRedisplay(); // Request another frame

    usleep(40000); // 40 milliseconds delay to control frame rate
}

void reshape(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100.0, (GLfloat)w / (GLfloat)h, 0.5, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Pendulum Clock Animation");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}




//bezier curve
#include <math.h>
#include <GL/glut.h>

int SCREEN_HEIGHT = 480;
int NUMPOINTS = 0;

class Point {
public:
    float x, y;
    void setxy(float x2, float y2) {
        x = x2;
        y = y2;
    }
    Point operator&(const Point &rPoint) {
        x = rPoint.x;
        y = rPoint.y;
        return *this;
    }
};

Point abc[3];

void myInit() {
    glClearColor(0.0, 0.0, 0.0, 0.0);       // Black background
    glColor3f(1.0f, 0.0, 0.0);              // Initial drawing color (Red)
    glPointSize(4.0);                       // Point size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 640, 0.0, 480.0);       // 2D orthographic projection
}

void drawDot(Point pt) {
    glBegin(GL_POINTS);
    glVertex2f(pt.x, pt.y);
    glEnd();
    glFlush();
}

void drawLine(Point p1, Point p2) {
    glBegin(GL_LINES);
    glVertex2f(p1.x, p1.y);
    glVertex2f(p2.x, p2.y);
    glEnd();
    glFlush();
}

Point drawBezier(Point A, Point B, Point C, double t) {
    Point P;
    P.x = pow((1 - t), 2) * A.x + 2 * t * (1 - t) * B.x + pow(t, 2) * C.x;
    P.y = pow((1 - t), 2) * A.y + 2 * t * (1 - t) * B.y + pow(t, 2) * C.y;
    return P;
}

void myMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        abc[NUMPOINTS].setxy((float)x, (float)(SCREEN_HEIGHT - y));
        NUMPOINTS++;

        if (NUMPOINTS == 3) {
            // Draw control points
            glColor3f(1.0, 0.0, 1.0); // Magenta
            drawDot(abc[0]);
            drawDot(abc[1]);
            drawDot(abc[2]);

            // Draw control polygon
            glColor3f(1.0, 1.0, 0.0); // Yellow
            drawLine(abc[0], abc[1]);
            drawLine(abc[1], abc[2]);

            // Draw Bezier curve
            Point POld = abc[0];
            for (double t = 0.0; t <= 1.0; t += 0.1) {
                Point P = drawBezier(abc[0], abc[1], abc[2], t);
                drawLine(POld, P);
                POld = P;
            }

            glColor3f(1.0, 0.0, 0.0); // Reset color to red
            NUMPOINTS = 0;
        }
    }
}

void myDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 150);
    glutCreateWindow("Bezier Curve");

    glutMouseFunc(myMouse);
    glutDisplayFunc(myDisplay);

    myInit();
    glutMainLoop();
    return 0;
}




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







//boundaryfill
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



