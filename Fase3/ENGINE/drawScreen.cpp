#include "drawScreen.h"

float camX, camY, camZ;
float radius = 250.0f;
float refX = 0, refY = 0, refZ = 0;
float alfa = 0.0f, beta = 0.5f;

int pass = 0;
int timebase = 0, frame = 0;

std::vector<xmlData> globalInfo;
GLuint* figuras;
GLuint* tamanhoFiguras;
GLuint numDeFiguras;
GLuint figurasDesenhadas;

float tt[50];
float elapsedTime[50];
float timeTranslate;

float angle[50];

float prevY[3] = { 0,1,0 };

typedef float float_array[3];


void buildRotMatrix(float* x, float* y, float* z, float* m) {

    m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
    m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
    m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
    m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}


void cross(float* a, float* b, float* res) {

    res[0] = a[1] * b[2] - a[2] * b[1];
    res[1] = a[2] * b[0] - a[0] * b[2];
    res[2] = a[0] * b[1] - a[1] * b[0];
}


void normalize(float* a) {

    float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0] / l;
    a[1] = a[1] / l;
    a[2] = a[2] / l;
}


float length(float* v) {

    float res = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    return res;

}

void multMatrixVector(float* m, float* v, float* res) {

    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += v[k] * m[j * 4 + k];
        }
    }

}

void getCatmullRomPoint(float t, float* p0, float* p1, float* p2, float* p3, float* pos, float* deriv) {

    // catmull-rom matrix
    float m[4][4] = { {-0.5f,  1.5f, -1.5f,  0.5f},
                        { 1.0f, -2.5f,  2.0f, -0.5f},
                        {-0.5f,  0.0f,  0.5f,  0.0f},
                        { 0.0f,  1.0f,  0.0f,  0.0f} };

    float px[4] = { p0[0],p1[0],p2[0],p3[0] };
    float py[4] = { p0[1],p1[1],p2[1],p3[1] };
    float pz[4] = { p0[2],p1[2],p2[2],p3[2] };

    // Compute A = M * P
    float a[3][4];
    multMatrixVector(m[0], px, a[0]);
    multMatrixVector(m[0], py, a[1]);
    multMatrixVector(m[0], pz, a[2]);
    // Compute pos = T * A
    float T[4] = { (float)pow((double)t,3), (float)pow((double)t,2), t, 1 };
    pos[0] = { T[0] * a[0][0] + T[1] * a[0][1] + T[2] * a[0][2] + T[3] * a[0][3] };
    pos[1] = { T[0] * a[1][0] + T[1] * a[1][1] + T[2] * a[1][2] + T[3] * a[1][3] };
    pos[2] = { T[0] * a[2][0] + T[1] * a[2][1] + T[2] * a[2][2] + T[3] * a[2][3] };

    // compute deriv = T' * A
    float d[4] = { (float)3 * pow((double)t,2), 2 * t, 1, 0 };
    deriv[0] = { d[0] * a[0][0] + d[1] * a[0][1] + d[2] * a[0][2] + d[3] * a[0][3] };
    deriv[1] = { d[0] * a[1][0] + d[1] * a[1][1] + d[2] * a[1][2] + d[3] * a[1][3] };
    deriv[2] = { d[0] * a[2][0] + d[1] * a[2][1] + d[2] * a[2][2] + d[3] * a[2][3] };

    // ...
}

// given  global t, returns the point in the curve
void getGlobalCatmullRomPoint(float gt, float* pos,float* deriv, float curvePoints[][3], int size) {

    float t = gt * size; // this is the real global t
    int index = floor(t);  // which segment
    t = t - index; // where within  the segment

    // indices store the points
    int indices[4];
    indices[0] = (index + size - 1) % size;
    indices[1] = (indices[0] + 1) % size;
    indices[2] = (indices[1] + 1) % size;
    indices[3] = (indices[2] + 1) % size;

    getCatmullRomPoint(t, curvePoints[indices[0]], curvePoints[indices[1]], curvePoints[indices[2]], curvePoints[indices[3]], pos, deriv);
}

int coordsToArray(std::vector<float> pontos, float curvePoints[][3]) {
    int i = 0;
    for (int k=0 ; k < pontos.size(); k+=3) {
        curvePoints[i][0] = pontos.at(k);
        curvePoints[i][1] = pontos.at(k+1);
        curvePoints[i][2] = pontos.at(k+2);
        i++;
    }
    return i;
}

void planetTranslate(xmlData nodo) {
        float planetRotTime = nodo.getTempoTr();

        float_array* curvePoints = new float_array[(nodo.getControlPoints().size()) / 3];
        int size = coordsToArray(nodo.getControlPoints(), curvePoints);

        float pos[3];
        float deriv[3];

        glBegin(GL_LINE_LOOP);
        int npoints = 100;
        for (int i = 0; i < npoints; i++) {
            getGlobalCatmullRomPoint((double)i / npoints, pos, deriv, curvePoints, size);
            glVertex3d(pos[0], pos[1], pos[2]);
        }
        glEnd();

        if (nodo.getFileName().at(0) == "teapot.3d") {
            getGlobalCatmullRomPoint(tt[figurasDesenhadas], pos, deriv, curvePoints, size); // obtém a posição da figura no instante tt 

            float x[3] = { deriv[0],deriv[1],deriv[2] };
            normalize(x);
            float z[3];
            float y[3];
            cross(x, prevY, z);
            normalize(z);
            cross(z, x, y);
            normalize(y);
            prevY[0] = y[0]; prevY[1] = y[1]; prevY[2] = y[2];

            float rotMatrix[16];
            buildRotMatrix(x, y, z, rotMatrix);

            if (timeTranslate < numDeFiguras) {
                elapsedTime[figurasDesenhadas] = 1 / (nodo.getTempoTr() * 100);
                timeTranslate++;
            }
            tt[figurasDesenhadas] += elapsedTime[figurasDesenhadas];
            glTranslatef(pos[0], pos[1], pos[2]);
            glMultMatrixf(rotMatrix);
        }
        else {
            getGlobalCatmullRomPoint(tt[figurasDesenhadas], pos, deriv, curvePoints, size);

            if (timeTranslate < numDeFiguras) {
                elapsedTime[figurasDesenhadas] = 1 / (nodo.getTempoTr() * 100);
                timeTranslate++;
            }
            tt[figurasDesenhadas] += elapsedTime[figurasDesenhadas];
            glTranslatef(pos[0], pos[1], pos[2]);
        }
}

void planetRotate(xmlData nodo) {
    float time = nodo.getTempoRt();
    angle[figurasDesenhadas] += 360 / (time * 1000);
    glRotatef(angle[figurasDesenhadas], nodo.getRotateX(), nodo.getRotateY(), nodo.getRotateZ());
}

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

    std::vector<xmlData> filhos = pai.getFilhos();
    for (unsigned int subNodeAt = 0; figurasDesenhadas < numDeFiguras && subNodeAt < filhos.size(); subNodeAt++) {
        glPushMatrix();

        xmlData nodo = filhos.at(subNodeAt);
        int numTRs = nodo.getNumTransformacoes();

        if (nodo.getFileName().at(0) == "lua.3d")
            glColor3f(0, 0, 1);
        if (nodo.getFileName().at(0) == "planeta.3d")
            glColor3f(0.9, 0.8, 0.7);
        if (nodo.getFileName().at(0) == "arco.3d")
            glColor3f(0, 1, 0);
        if (nodo.getFileName().at(0) == "teapot.3d")
            glColor3f(1, 0, 0);

        for (unsigned int k = 1; k <= numTRs; k++) {
            if (nodo.getOrdemTr() == k) {
                if (nodo.getTempoTr() == 0)
                    glTranslatef(nodo.getTranslateX(), nodo.getTranslateY(), nodo.getTranslateZ());
                else
                    planetTranslate(nodo);
            }
            if (nodo.getOrdemRt() == k) {
                if (nodo.getTempoRt() == 0)
                    glRotatef(nodo.getRotateAngulo(), nodo.getRotateX(), nodo.getRotateY(), nodo.getRotateZ());
                else
                    planetRotate(nodo);
            }
            if (nodo.getOrdemSc() == k)
                glScalef(nodo.getScaleX(), nodo.getScaleY(), nodo.getScaleZ());
        }

        glBindBuffer(GL_ARRAY_BUFFER, figuras[figurasDesenhadas]);
        glVertexPointer(3, GL_FLOAT, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, tamanhoFiguras[figurasDesenhadas]);
        
        figurasDesenhadas++;
        desenharFiguras(nodo);

        glPopMatrix();
    }
}
void desenharFiguras() {

    glColor3f(1, 1, 1);
    for (unsigned int mainNodeAt = 0; figurasDesenhadas < numDeFiguras && mainNodeAt < globalInfo.size(); mainNodeAt++) {
        glPushMatrix();
        
        if (mainNodeAt == 0) {
            glColor3f(1, 0.5, 0);
            //glColor3f(0.9, 0.8, 0.7);
        }

        xmlData nodo = globalInfo.at(mainNodeAt);
        int numTRs = nodo.getNumTransformacoes();
        
        for (unsigned int k = 1; k <= numTRs; k++) {
            if (nodo.getOrdemTr() == k) {
                if(nodo.getTempoTr() == 0)
                    glTranslatef(nodo.getTranslateX(), nodo.getTranslateY(), nodo.getTranslateZ());
                else {
                    planetTranslate(nodo);
                }
            }
            if (nodo.getOrdemRt() == k) {
                if (nodo.getTempoRt() == 0)
                    glRotatef(nodo.getRotateAngulo(), nodo.getRotateX(), nodo.getRotateY(), nodo.getRotateZ());
                else {
                    planetRotate(nodo);
                }
            }

            if (nodo.getOrdemSc() == k)
                glScalef(nodo.getScaleX(), nodo.getScaleY(), nodo.getScaleZ());
        }

        glBindBuffer(GL_ARRAY_BUFFER, figuras[figurasDesenhadas]);
        glVertexPointer(3, GL_FLOAT, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, tamanhoFiguras[figurasDesenhadas]);

        figurasDesenhadas++;
        desenharFiguras(nodo);

        glPopMatrix();

        glColor3f(1, 1, 1);
    }
    figurasDesenhadas = 0;
}

void renderScene(void) {

    static float glutTime = 0; // tempo que já passou
    float fps;
    int time;
    char s[64];

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (pass == 0) {
        spherical2cartesian();
        memset(tt, 0, 50);
        memset(elapsedTime, 0, 50);
        memset(angle, 0, 50);
        timeTranslate = 0;
        pass++;
    }
    // set the camera
    glLoadIdentity();
    gluLookAt(camX+refX,camY+refY,camZ+refZ,
              refX,refY,refZ,
              0.0f,1.0f,0.0f);
    
    frame++;
    time = glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000) {
        fps = frame * 1000.0 / (time - timebase);
        timebase = time;
        frame = 0;
        sprintf(s, "FPS: %f6.2", fps);
        glutSetWindowTitle(s);
    }

    desenharFiguras();

    // End of frame
    glutSwapBuffers();
    glutTime += 0.001;
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

    for (unsigned int subNodeAt = 0; (*figurasInMemory < numDeFiguras) && (subNodeAt < filhos.size()); subNodeAt++) {
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
        *figurasInMemory += 1;
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

    for (int mainNodeAt = 0; figurasInMemory < numDeFiguras; mainNodeAt++) {
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
        figurasInMemory++;
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
    glutIdleFunc(renderScene); // permite que aja sozinho

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
