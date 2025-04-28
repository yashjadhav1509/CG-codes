
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

