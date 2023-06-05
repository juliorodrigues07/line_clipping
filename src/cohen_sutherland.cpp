#include "cohen_sutherland.hpp"

GLfloat windowWidth;
GLfloat windowHeight;

int state(double x, double y, double x_min, double x_max, double y_min, double y_max) {

    unsigned char code = 0;

    if (y > y_max)
        code += ABOVE;
    else if (y < y_min)
        code += UNDER;

    if (x > x_max)
        code += RIGHT;
    else if (x < x_min)
        code += LEFT;

    return code;
}

void lineClip(double x_begin, double y_begin, double x_end, double y_end,
               double x_min, double x_max, double y_min, double y_max) {

    double x, y;
    unsigned char out;
    bool done = false;
    bool accept = false;

    unsigned char outcode0 = state(x_begin, y_begin, x_min, x_max, y_min, y_max);
    unsigned char outcode1 = state(x_end, y_end, x_min, x_max, y_min, y_max);

    do {

        // Trivial accept
        if (outcode0 == 0 && outcode1 == 1) {
            accept = true;
            done = true;
        }

        // Trivial reject
        else if ((outcode0 & outcode1) != 0)
            done = true;

        // Discard outer part
        else {

            // Picking out vertex
            out = (outcode0 != 0) ? outcode0 : outcode1;

            // Discards above
            if (out & ABOVE) {
                x = x_begin + (x_end - x_begin) * (y_max - y_begin) / (y_end - y_begin);
                y = y_max;
            }

            // Discards under
            else if (out & UNDER) {
                x = x_begin + (x_end - x_begin) * (y_min - y_begin) / (y_end - y_begin);
                y = y_min;
            }

            // Discards right
            else if (out & RIGHT) {
                y = y_begin + (y_end - y_begin) * (x_max - x_begin) / (x_end - x_begin);
                x = x_max;
            }

            // Discards left
            else if (out & LEFT) {
                y = y_begin + (y_end - y_begin) * (x_min - x_begin) / (x_end - x_begin);
                x = x_min;
            }

            if (out == outcode0) {
                x_begin = x;
                y_begin = y;
                outcode0 = state(x_begin, y_begin, x_min, x_max, y_min, y_max);
            } else {
                x_end = x;
                y_end = y;
                outcode1 = state(x_end, y_end, x_min, x_max, y_min, y_max);
            }
        }

    } while (!done);

    if (accept) {
        glBegin(GL_LINES);
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(x_begin, y_begin, 0.0f);
            glVertex3f(x_end, y_end, 0.0f);
        glEnd();
    }
}

void draw(void) {

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    glutSwapBuffers();
}

void myInit(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void changeWindowSize(GLsizei w, GLsizei h) {

    if (h == 0)
        h = 1;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (w <= h) {
        windowHeight = 250.0f * h / w;
        windowWidth = 250.0f;
    } else {
        windowWidth = 250.0f * w / h;
        windowHeight = 250.0f;
    }

    gluOrtho2D(0.0f, windowWidth, 0.0f, windowHeight);
}
