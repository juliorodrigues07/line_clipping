#include "cohen_sutherland.h"

char *file_name;
int height, width;

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char **argv) {

    if (argc != 4)
        error("Usage: ./clip <file_name> <screen_height> <screen_width>");

    file_name = argv[1];
    height = atoi(argv[2]);
    width = atoi(argv[3]);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Cohen Sutherland");
    glutDisplayFunc(draw);

    glutReshapeFunc(changeWindowSize);
    myInit();

    glutMainLoop();
    return 0;
}
