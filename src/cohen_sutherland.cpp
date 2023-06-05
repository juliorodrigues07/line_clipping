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

    double first_x = x_begin, first_y = y_begin;
    double second_x = x_end, second_y = y_end;

    double x, y;
    bool accept = false, done = false;

    unsigned char out;
    unsigned char outcode0 = state(x_begin, y_begin, x_min, x_max, y_min, y_max);
    unsigned char outcode1 = state(x_end, y_end, x_min, x_max, y_min, y_max);

    glColor3f(1.0f, 0.0f, 0.0f);
    do {

        // Trivial acccept
        if (outcode0 == 0 && outcode1 == 0)
        {
            accept = true;
            done = true;
        }

        // Trivial reject
        else if ((outcode0 & outcode1) != 0)
            done = true;

        // Discard outer part
        else
        {
            // Picking outer vertex
            out = (outcode0 != 0) ? outcode0 : outcode1;

            // Discards above
            if (out & ABOVE)
            {
                x = x_begin + (x_end - x_begin) * (y_max - y_begin) / (y_end - y_begin);
                y = y_max;
            }

            // Discards under
            else if (out & UNDER)
            {
                x = x_begin + (x_end - x_begin) * (y_min - y_begin) / (y_end - y_begin);
                y = y_min;
            }

            // Discards rigjt
            else if (out & RIGHT)
            {
                y = y_begin + (y_end - y_begin) * (x_max - x_begin) / (x_end - x_begin);
                x = x_max;
            }

            // Discards left
            else if (out & LEFT)
            {
                y = y_begin + (y_end - y_begin) * (x_min - x_begin) / (x_end - x_begin);
                x = x_min;
            }

            // Gradually "cuts" the external lines (towards the line center)
            if (out == outcode0)
            {
                x_begin = x;
                y_begin = y;
                outcode0 = state(x_begin, y_begin, x_min, x_max, y_min, y_max);

                glBegin(GL_LINES);
                    glVertex3f(first_x, first_y, 0.0f);
                    glVertex3f(x_begin, y_begin, 0.0f);
                glEnd();
            }
            else
            {
                x_end = x;
                y_end = y;
                outcode1 = state(x_end, y_end, x_min, x_max, y_min, y_max);

                glBegin(GL_LINES);
                    glVertex3f(x_end, y_end, 0.0f);
                    glVertex3f(second_x, second_y, 0.0f);
                glEnd();
            }
        }
    } while (!done);

    if (accept)
    {
        // Accepted line (inside area)
        glBegin(GL_LINES);
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(x_begin, y_begin, 0.0f);
            glVertex3f(x_end, y_end, 0.0f);
        glEnd();
    }
    else
    {
        // Rejected line (totally outside)
        glBegin(GL_LINES);
            glVertex3f(first_x, first_y, 0.0f);
            glVertex3f(second_x, second_y, 0.0f);
        glEnd();
    }
}

void draw(void) {

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    unsigned int n_lines;
    double x_min, x_max, y_min, y_max;
    FILE *file = fopen(file_name, "r");

    if (file == NULL)
        error("Could not open the file.\n");

    fscanf(file, "%lf %lf %lf %lf %u\n", &x_min, &x_max, &y_min, &y_max, &n_lines);

    if (x_min < 0 || x_max > width || y_min < 0 || y_max > height) {
        printf("Window: %d x %d\n", width, height);
        error("Cut area must be smaller than the screen.\n");
    }

    line *lines = (line *) malloc(n_lines * sizeof(line));
    for (int i = 0; i < n_lines; i++)
        fscanf(file, "%lf %lf %lf %lf\n", &lines[i].x_begin, &lines[i].y_begin, &lines[i].x_end, &lines[i].y_end);

    fclose(file);

    // Cut area
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_STRIP);
        glVertex3d(x_min, y_min, 0.0);
        glVertex3d(x_min, y_max, 0.0);
        glVertex3d(x_max, y_max, 0.0);
        glVertex3d(x_max, y_min, 0.0);
        glVertex3d(x_min, y_min, 0.0);
    glEnd();

    // Draws all the lines and clips them
    for (int i = 0; i < n_lines; i++) {

        glBegin(GL_LINES);
            glVertex3f(lines[i].x_begin, lines[i].y_begin, 0.0f);
            glVertex3f(lines[i].x_end, lines[i].y_end, 0.0f);
        glEnd();

        lineClip(lines[i].x_begin, lines[i].y_begin, lines[i].x_end, lines[i].y_end, x_min, x_max, y_min, y_max);
    }

    free(lines);
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
