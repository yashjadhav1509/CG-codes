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
