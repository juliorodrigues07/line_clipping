#include "cohen_sutherland.hpp"

int main(int argc, char **argv) {

    if (argc != 1)
        error("Usage: ./clip");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Cohen Sutherland");
    glutDisplayFunc(draw);

    glutReshapeFunc(changeWindowSize);
    myInit();

    glutMainLoop();
    return 0;
}