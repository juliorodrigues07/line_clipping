#include <stdio.h>
#include <stdbool.h>
#include <GL/glut.h>

#define ABOVE 8
#define UNDER 4
#define RIGHT 2
#define LEFT 1

typedef struct {
    double x_begin;
    double y_begin;
    double x_end;
    double y_end;
} line;

extern char *file_name;
extern int height, width;

int state(double x, double y, double x_min, double x_max, double y_min, double y_max);
void lineClip(double x_begin, double y_begin, double x_end, double y_end,
           double x_min, double x_max, double y_min, double y_max);

void draw(void);
void myInit(void);
void changeWindowSize(GLsizei w, GLsizei h);

void error(const char *msg);
