#include "drawScreen.h"

float camX=5.0, camY=5.0, camZ=5.0;
float radius = 15.0f;
float alfa = 0.0f, beta = 0.5f;

std::vector<std::vector<triangulo>> globalInfo;

void spherical2cartesian() {
    camX = radius * sin(alfa) * cos(beta);
    camZ = radius * cos(alfa) * cos(beta);
    camY = radius * sin(beta);
}

void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if(h == 0)
        h = 1;

    // compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

void desenharFiguras() {

    glColor3f(1, 1, 1);

    for (unsigned int i = 0; i < globalInfo.size(); i++) {

        std::vector<triangulo> figura = globalInfo.at(i);
        for (unsigned int k = 0; k < figura.size(); k++) {

            triangulo tr = figura.at(k);

            glBegin(GL_TRIANGLES);
            glVertex3f(tr.getVerticeX1(), tr.getVerticeY1(), tr.getVerticeZ1());
            glVertex3f(tr.getVerticeX2(), tr.getVerticeY2(), tr.getVerticeZ2());
            glVertex3f(tr.getVerticeX3(), tr.getVerticeY3(), tr.getVerticeZ3());
            glEnd();
        }
    }
}

void renderScene(void) {

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    gluLookAt(camX,camY,camZ,
              0.0,0.0,0.0,
              0.0f,1.0f,0.0f);

    //drawCylinder(2,4,30);

    desenharFiguras();

    // End of frame
    glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here

}


void processSpecialKeys(int key, int xx, int yy) {

// put code to process special keys in here
    switch (key) {
        case(GLUT_KEY_RIGHT):
            alfa -= 0.01; break;
        case(GLUT_KEY_LEFT):
            alfa += 0.01; break;
        case(GLUT_KEY_UP):
            beta += 0.01;
            if (beta > 1.5f)
                beta = 1.5f;
            break;
        case(GLUT_KEY_DOWN):
            beta -= 0.01;
            if (beta < -1.5f)
                beta = -1.5f;
            break;
    }
    spherical2cartesian();
    glutPostRedisplay();
}


void startDrawing(int argc, char **argv, std::vector<std::vector<triangulo>> figuras) {

    globalInfo = figuras;

// init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutCreateWindow("CG@DI-UM");

// Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

// Callback registration for keyboard processing
    glutKeyboardFunc(processKeys);
    glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

// enter GLUT's main cycle
    glutMainLoop();
}
