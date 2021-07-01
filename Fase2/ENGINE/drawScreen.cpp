#include "drawScreen.h"

float camX, camY, camZ;
float radius = 250.0f;
float refX = 0, refY = 0, refZ = 0;
float alfa = 0.0f, beta = 0.5f;

int pass = 0;

std::vector<xmlData> globalInfo;
GLuint* figuras;
GLuint* tamanhoFiguras;
GLuint numDeFiguras;
GLuint figurasDesenhadas;

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
    gluPerspective(45.0f ,ratio, 1.0f ,10000.0f);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

void desenharFiguras(xmlData pai) {
    
    glColor3f(0, 0, 1);

    std::vector<xmlData> filhos = pai.getFilhos();
    for (unsigned int subNodeAt = 0; figurasDesenhadas < numDeFiguras && subNodeAt < filhos.size(); figurasDesenhadas++, subNodeAt++) {
        glPushMatrix();

        xmlData nodo = filhos.at(subNodeAt);
        int numTRs = nodo.getNumTransformacoes();

        for (unsigned int k = 1; k <= numTRs; k++) {
            if (nodo.getOrdemTr() == k)
                glTranslatef(nodo.getTranslateX(), nodo.getTranslateY(), nodo.getTranslateZ());

            if (nodo.getOrdemRt() == k)
                glRotatef(nodo.getRotateAngulo(), nodo.getRotateX(), nodo.getRotateY(), nodo.getRotateZ());

            if (nodo.getOrdemSc() == k)
                glScalef(nodo.getScaleX(), nodo.getScaleY(), nodo.getScaleZ());
        }

        glBindBuffer(GL_ARRAY_BUFFER, figuras[figurasDesenhadas]);
        glVertexPointer(3, GL_FLOAT, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, tamanhoFiguras[figurasDesenhadas]);

        desenharFiguras(nodo);

        glPopMatrix();
    }
}
void desenharFiguras() {

    glColor3f(1, 1, 1);
    for (unsigned int mainNodeAt = 0; figurasDesenhadas < numDeFiguras && mainNodeAt < globalInfo.size(); figurasDesenhadas++,mainNodeAt++) {
        glPushMatrix();
        
        if (mainNodeAt == 0) {
            glColor3f(1, 0.5, 0);
        }

        xmlData nodo = globalInfo.at(mainNodeAt);
        int numTRs = nodo.getNumTransformacoes();
        
        for (unsigned int k = 1; k <= numTRs; k++) {
            if (nodo.getOrdemTr() == k)
                glTranslatef(nodo.getTranslateX(), nodo.getTranslateY(), nodo.getTranslateZ());

            if (nodo.getOrdemRt() == k)
                glRotatef(nodo.getRotateAngulo(), nodo.getRotateX(), nodo.getRotateY(), nodo.getRotateZ());

            if (nodo.getOrdemSc() == k)
                glScalef(nodo.getScaleX(), nodo.getScaleY(), nodo.getScaleZ());
        }

        glBindBuffer(GL_ARRAY_BUFFER, figuras[figurasDesenhadas]);
        glVertexPointer(3, GL_FLOAT, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, tamanhoFiguras[figurasDesenhadas]);

        desenharFiguras(nodo);

        glPopMatrix();

        glColor3f(1, 1, 1);
    }
    figurasDesenhadas = 0;
}

void renderScene(void) {
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (pass == 0) {
        spherical2cartesian();
        pass++;
    }
    // set the camera
    glLoadIdentity();
    gluLookAt(camX+refX,camY+refY,camZ+refZ,
              refX,refY,refZ,
              0.0f,1.0f,0.0f);

    desenharFiguras();

    // End of frame
    glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here
    switch (c) {
    case 'w': {
        refZ -= 5 * cos(alfa);
        refX -= 5 * sin(alfa);
    } break;
    case 's': {
        refZ += 5 * cos(alfa);
        refX += 5 * sin(alfa);
    } break;

    case 'd': {
        refX += 5 * cos(alfa);
        refZ += 5 * (-sin(alfa));
    } break;
    case 'a': {
        refX -= 5 * cos(alfa);
        refZ -= 5 * (-sin(alfa));
    } break;
    case 'q': {
        refY += 3;
    } break;
    case 'e': {
        refY -= 3;
    } break;
    case '-': {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } break;
    case '.': {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    } break;
    case ',': {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } break;
    }

    glutPostRedisplay();
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

        case GLUT_KEY_PAGE_DOWN: radius -= 5.0f;
            if (radius < 1.0f)
                radius = 1.0f;
            break;

        case GLUT_KEY_PAGE_UP: radius += 5.0f; break;
    }
    spherical2cartesian();
    glutPostRedisplay();
}

void getTotalFiguras(std::vector<xmlData> f) {
    for (unsigned int i = 0; i < f.size(); i++) {
        xmlData pai = f.at(i);
        numDeFiguras++;
        std::vector<xmlData> filhos = pai.getFilhos();
        getTotalFiguras(filhos);
    }
}
void toBuffers(std::vector<xmlData> filhos, int* figurasInMemory) {

    std::vector<float> pontos;
    int size = 0;

    for (unsigned int subNodeAt = 0; (*figurasInMemory < numDeFiguras) && (subNodeAt < filhos.size()); *figurasInMemory+=1 , subNodeAt++) {
        xmlData ficheiro = filhos.at(subNodeAt);
        std::vector<triangulo> figura = ficheiro.getPontos();

        for (unsigned int k = 0; k < figura.size(); k++) {
            triangulo tr = figura.at(k);
            pontos.push_back(tr.getVerticeX1());
            pontos.push_back(tr.getVerticeY1());
            pontos.push_back(tr.getVerticeZ1());
            pontos.push_back(tr.getVerticeX2());
            pontos.push_back(tr.getVerticeY2());
            pontos.push_back(tr.getVerticeZ2());
            pontos.push_back(tr.getVerticeX3());
            pontos.push_back(tr.getVerticeY3());
            pontos.push_back(tr.getVerticeZ3());
        }

        int size = pontos.size();
        tamanhoFiguras[*figurasInMemory] = size;
        glGenBuffers(1, &figuras[*figurasInMemory]);
        glBindBuffer(GL_ARRAY_BUFFER, figuras[*figurasInMemory]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, &pontos[0], GL_STATIC_DRAW);
        pontos.clear();
        toBuffers(ficheiro.getFilhos(), figurasInMemory);
    }
}
void toBuffers(std::vector<xmlData> figs) {
       
    numDeFiguras = 0;
    figurasDesenhadas = 0;
    
    getTotalFiguras(figs);

    globalInfo = figs;
    figuras = new GLuint[numDeFiguras];
    tamanhoFiguras = new GLuint[numDeFiguras];
    std::vector<float> pontos;
    int figurasInMemory = 0;
    int size = 0;

    for (int mainNodeAt = 0; figurasInMemory < numDeFiguras; figurasInMemory++,mainNodeAt++) {
        xmlData ficheiro = figs.at(mainNodeAt);
        std::vector<triangulo> figura = ficheiro.getPontos();
        
        for (unsigned int k = 0; k < figura.size(); k++) {
            triangulo tr = figura.at(k);
            pontos.push_back(tr.getVerticeX1());
            pontos.push_back(tr.getVerticeY1());
            pontos.push_back(tr.getVerticeZ1());
            pontos.push_back(tr.getVerticeX2());
            pontos.push_back(tr.getVerticeY2());
            pontos.push_back(tr.getVerticeZ2());
            pontos.push_back(tr.getVerticeX3());
            pontos.push_back(tr.getVerticeY3());
            pontos.push_back(tr.getVerticeZ3());
        }
        
        int size = pontos.size();
        tamanhoFiguras[figurasInMemory] = size;
        glGenBuffers(1, &figuras[figurasInMemory]);
        glBindBuffer(GL_ARRAY_BUFFER, figuras[figurasInMemory]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, &pontos[0], GL_STATIC_DRAW);
        pontos.clear();
        toBuffers(ficheiro.getFilhos(), &figurasInMemory);
    }
}

void startDrawing(int argc, char **argv, std::vector<xmlData> figs) {

// init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutCreateWindow("CG@DI-UM");
    
    glewInit();

// Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

// Callback registration for keyboard processing
    glutKeyboardFunc(processKeys);
    glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

// Funções de setup antes do main cycle
    toBuffers(figs);

// enter GLUT's main cycle
    glutMainLoop();
}
