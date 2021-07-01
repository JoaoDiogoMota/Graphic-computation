#include "drawScreen.h"

float camX, camY, camZ;
float radius = 250.0f;
float refX = 0, refY = 0, refZ = 0;
float alfa = 0.0f, beta = 0.5f;
int startX, startY, tracking = 0;

int pass = 0;
int timebase = 0, frame = 0;

std::vector<xmlData> globalInfo;
std::vector<light> luzGlobal;

GLuint* figuras;
GLuint* normais;
GLuint* coordenadas;
GLuint* tamanhoFiguras;
GLuint* texture;

GLuint numDeFiguras;
GLuint figurasDesenhadas;

float tt[50];
float elapsedTime[50];
float timeTranslate;

float angle[50];

float prevY[3] = { 0,1,0 };

typedef float float_array[3];

float defD[4] = { 0.8, 0.8, 0.8, 1.0 };
float defS[4] = { 0.0, 0.0, 0.0, 1.0 };
float defE[4] = { 0.0, 0.0, 0.0, 1.0 };
float defA[4] = { 0.2, 0.2, 0.2, 1.0 };

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

        //glBegin(GL_LINE_LOOP);
        int npoints = 100;
        for (int i = 0; i < npoints; i++) {
            getGlobalCatmullRomPoint((double)i / npoints, pos, deriv, curvePoints, size);
            glVertex3d(pos[0], pos[1], pos[2]);
        }
        glEnd();

        if (nodo.getFileName().at(0) == "teapot") {
            getGlobalCatmullRomPoint(tt[figurasDesenhadas], pos, deriv, curvePoints, size);

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
                double ttt = glutGet(GLUT_ELAPSED_TIME);
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
                double ttt = glutGet(GLUT_ELAPSED_TIME);
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

        for (unsigned int k = 1; k <= numTRs; k++) {
            if (nodo.getOrdemTr() == k) {
                if (nodo.getTempoTr() == 0)
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

        for (unsigned int i = 0; i < nodo.getFileName().size(); i++) {
            obj_Light ls = nodo.getMateriais().at(i);

            if (ls.getDV() == 1) {
                float put[4] = { ls.getDR(), ls.getDG(), ls.getDB(), 1.0 };
                glMaterialfv(GL_FRONT, GL_DIFFUSE, put);
            }
            if (ls.getSV() == 1) {
                float put[4] = { ls.getSR(), ls.getSG(), ls.getSB(), 1.0 };
                glMaterialfv(GL_FRONT, GL_SPECULAR, put);
                glMaterialf(GL_FRONT, GL_SHININESS, ls.getShine());
            }
            if (ls.getEV() == 1) {
                float put[4] = { ls.getER(), ls.getEG(), ls.getEB(), 1.0 };
                glMaterialfv(GL_FRONT, GL_EMISSION, put);
            }
            if (ls.getAV() == 1) {
                float put[4] = { ls.getAR(), ls.getAG(), ls.getAB(), 1.0 };
                glMaterialfv(GL_FRONT, GL_AMBIENT, put);
            }

            glBindTexture(GL_TEXTURE_2D, texture[figurasDesenhadas]);

            glBindBuffer(GL_ARRAY_BUFFER, normais[figurasDesenhadas]);
            glNormalPointer(GL_FLOAT, 0, 0);

            glBindBuffer(GL_ARRAY_BUFFER, figuras[figurasDesenhadas]);
            glVertexPointer(3, GL_FLOAT, 0, 0);

            if (!(nodo.getFileName().at(i) == "none")) {
                glBindBuffer(GL_ARRAY_BUFFER, coordenadas[figurasDesenhadas]);
                glTexCoordPointer(2, GL_FLOAT, 0, 0);
            }

            glDrawArrays(GL_TRIANGLES, 0, tamanhoFiguras[figurasDesenhadas]);

            glBindTexture(GL_TEXTURE_2D, 0);

            if (ls.getDV() == 1)
                glMaterialfv(GL_FRONT, GL_DIFFUSE, defD);
            if (ls.getEV() == 1)
                glMaterialfv(GL_FRONT, GL_EMISSION, defE);
            if (ls.getSV() == 1)
                glMaterialfv(GL_FRONT, GL_SPECULAR, defS);
            if (ls.getAV() == 1)
                glMaterialfv(GL_FRONT, GL_AMBIENT, defA);

            figurasDesenhadas++;
        }
        desenharFiguras(nodo);

        glPopMatrix();
    }
}

void desenharFiguras() {

    for (unsigned int mainNodeAt = 0; figurasDesenhadas < numDeFiguras && mainNodeAt < globalInfo.size(); mainNodeAt++) {
        glPushMatrix();

        xmlData nodo = globalInfo.at(mainNodeAt);
        int numTRs = nodo.getNumTransformacoes();

        for (unsigned int k = 1; k <= numTRs; k++) {
            if (nodo.getOrdemTr() == k) {
                if (nodo.getTempoTr() == 0)
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
        
        for(unsigned int i = 0; i < nodo.getFileName().size(); i++){
            obj_Light ls = nodo.getMateriais().at(i);

            if (ls.getDV() == 1) {
                float put[4] = { ls.getDR(), ls.getDG(), ls.getDB(), 1.0 };
                glMaterialfv(GL_FRONT, GL_DIFFUSE, put);
            }
            if (ls.getSV() == 1) {
                float put[4] = { ls.getSR(), ls.getSG(), ls.getSB(), 1.0 };
                glMaterialfv(GL_FRONT, GL_SPECULAR, put);
                glMaterialf(GL_FRONT, GL_SHININESS, ls.getShine());
            }
            if (ls.getEV() == 1) {
                float put[4] = { ls.getER(), ls.getEG(), ls.getEB(), 1.0 };
                glMaterialfv(GL_FRONT, GL_EMISSION, put);
            }
            if (ls.getAV() == 1) {
                float put[4] = { ls.getAR(), ls.getAG(), ls.getAB(), 1.0 };
                glMaterialfv(GL_FRONT, GL_AMBIENT, put);
            }

            glBindTexture(GL_TEXTURE_2D, texture[figurasDesenhadas]);

            glBindBuffer(GL_ARRAY_BUFFER, normais[figurasDesenhadas]);
            glNormalPointer(GL_FLOAT, 0, 0);

            glBindBuffer(GL_ARRAY_BUFFER, figuras[figurasDesenhadas]);
            glVertexPointer(3, GL_FLOAT, 0, 0);

            if (!(nodo.getFileName().at(i) == "none")) {
                glBindBuffer(GL_ARRAY_BUFFER, coordenadas[figurasDesenhadas]);
                glTexCoordPointer(2, GL_FLOAT, 0, 0);
            }

            glDrawArrays(GL_TRIANGLES, 0, tamanhoFiguras[figurasDesenhadas]);
       
            glBindTexture(GL_TEXTURE_2D, 0);
            
            if (ls.getDV() == 1)
                glMaterialfv(GL_FRONT, GL_DIFFUSE, defD);
            if (ls.getEV() == 1)
                glMaterialfv(GL_FRONT, GL_EMISSION, defE);
            if (ls.getSV() == 1)
                glMaterialfv(GL_FRONT, GL_SPECULAR, defS);
            if (ls.getAV() == 1)
                glMaterialfv(GL_FRONT, GL_AMBIENT, defA);

            figurasDesenhadas++;
        }
        desenharFiguras(nodo);

        glPopMatrix();
    }
    figurasDesenhadas = 0;
}

void applyLight() {

    for (int i = 0; i < luzGlobal.size(); i++) {
        glEnable(GL_LIGHT0 + i);
        light lg = luzGlobal.at(i);

        std::string lType = lg.getTipo();

        if (lType.compare("POINT") == 0) {

            GLfloat amb[4] = { 0.1, 0.1, 0.1, 1.0 };
            GLfloat diff[4] = { 1.0, 1.0, 1.0, 1.0 };
            float light[4] = { lg.getPointX(), lg.getPointY(), lg.getPointZ(), 1.0 };

            glLightfv(GL_LIGHT0 + i, GL_POSITION, light);
            glLightfv(GL_LIGHT0 + i, GL_AMBIENT, amb);
            glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, diff);
        }

        if (lType.compare("DIRECTIONAL") == 0) {

            GLfloat amb[4] = { 0.1, 0.1, 0.1, 1.0 };
            GLfloat diff[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
            float light[4] = { lg.getDirX(), lg.getDirY(), lg.getDirZ(), 0.0 };

            glLightfv(GL_LIGHT0 + i, GL_POSITION, light);
            glLightfv(GL_LIGHT0 + i, GL_AMBIENT, amb);
            glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, diff);
        }

        if (lType.compare("SPOT") == 0) {

            GLfloat amb[4] = { 0.2, 0.2, 0.2, 1.0 };
            GLfloat diff[4] = { 1.0, 1.0, 1.0, 1.0 };
            float spotPos[4] = { lg.getPointX(), lg.getPointY(), lg.getPointZ(),1.0 };
            float spotDir[3] = { lg.getDirX(), lg.getDirY(), lg.getDirZ() };

            glLightfv(GL_LIGHT0 + i, GL_POSITION, spotPos);
            glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, diff);
            glLightfv(GL_LIGHT0 + i, GL_AMBIENT, amb);
            glLightfv(GL_LIGHT0 + i, GL_SPOT_DIRECTION, spotDir);
            glLightf(GL_LIGHT0 + i, GL_SPOT_CUTOFF, lg.getPhi()); // [0,90] ou 180
            glLightf(GL_LIGHT0 + i, GL_SPOT_EXPONENT, lg.getTheta()); // [0,128]
        }
    }
}

void renderScene(void) {

    static float glutTime = 0;
    float fps;
    int time;
    char s[64];

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glMaterialf(GL_FRONT, GL_SHININESS, 100);

    if (pass == 0) {
        memset(tt, 0, 50);
        memset(elapsedTime, 0, 50);
        memset(angle, 0, 50);
        timeTranslate = 0;
        pass++;
    }

    // set the camera
    glLoadIdentity();
    gluLookAt(camX + refX, camY + refY, camZ + refZ,
              refX, refY, refZ,
              0.0f, 1.0f, 0.0f);

    //set The Light
    applyLight();

    frame++;
    time = glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000) {
        fps = frame * 1000.0 / (time - timebase);
        timebase = time;
        frame = 0;
        sprintf(s, "FPS: %f6.2", fps);
        glutSetWindowTitle(s);
    }

    //glColor3f(1, 1, 1);
    desenharFiguras();

    // End of frame
    glutSwapBuffers();
    glutTime += 0.001;
}

void processMouseButtons(int button, int state, int xx, int yy)
{
    if (state == GLUT_DOWN) {
        startX = xx;
        startY = yy;
        if (button == GLUT_LEFT_BUTTON)
            tracking = 1;
        else if (button == GLUT_RIGHT_BUTTON)
            tracking = 2;
        else
            tracking = 0;
    }
    else if (state == GLUT_UP) {
        if (tracking == 1) {
            alfa += (xx - startX);
            beta += (yy - startY);
        }
        else if (tracking == 2) {

            radius -= yy - startY;
            if (radius < 3)
                radius = 3.0;
        }
        tracking = 0;
    }
}

void processMouseMotion(int xx, int yy)
{
    int deltaX, deltaY;
    int alphaAux, betaAux;
    int rAux;

    if (!tracking)
        return;

    deltaX = xx - startX;
    deltaY = yy - startY;

    if (tracking == 1) {

        alphaAux = alfa + deltaX;
        betaAux = beta + deltaY;

        if (betaAux > 85.0)
            betaAux = 85.0;
        else if (betaAux < -85.0)
            betaAux = -85.0;

        rAux = radius;
    }
    else if (tracking == 2) {

        alphaAux = alfa;
        betaAux = beta;
        rAux = radius - deltaY;
        if (rAux < 3)
            rAux = 3;
    }

    camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
    camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
    camY = rAux * sin(betaAux * 3.14 / 180.0);
}

void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here
    switch (c) {
    case 'w': {
        refZ -= 5 * cos(alfa * 3.14 / 180.0);
        refX -= 5 * sin(alfa * 3.14 / 180.0);
    } break;
    case 's': {
        refZ += 5 * cos(alfa * 3.14 / 180.0);
        refX += 5 * sin(alfa * 3.14 / 180.0);
    } break;

    case 'd': {
        refX += 5 * cos(alfa * 3.14 / 180.0);
        refZ += 5 * (-sin(alfa * 3.14 / 180.0));
    } break;
    case 'a': {
        refX -= 5 * cos(alfa * 3.14 / 180.0);
        refZ -= 5 * (-sin(alfa * 3.14 / 180.0));
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

void getTotalFiguras(std::vector<xmlData> f) {
    for (unsigned int i = 0; i < f.size(); i++) {
        xmlData pai = f.at(i);
        numDeFiguras += pai.getFileName().size();
        std::vector<xmlData> filhos = pai.getFilhos();
        getTotalFiguras(filhos);
    }
}

void initTexture(obj_Light info, int figurasInMemory) {
    
    if(!(info.getTexture() == "none")){
        unsigned int imagem, width, height;
        unsigned char* texData;

        std::string tex = "../../SistemaSolar/" + info.getTexture();

        ilInit();
        ilEnable(IL_ORIGIN_SET);
        ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
        ilGenImages(1, &imagem);
        ilBindImage(imagem);
        ilLoadImage((ILstring)tex.c_str());
        width = ilGetInteger(IL_IMAGE_WIDTH);
        height = ilGetInteger(IL_IMAGE_HEIGHT);
        ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
        texData = ilGetData();
        
        glGenTextures(1, &texture[figurasInMemory]);

        glBindTexture(GL_TEXTURE_2D, texture[figurasInMemory]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
        glGenerateMipmap(GL_TEXTURE_2D);
    
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}

void toBuffers(std::vector<xmlData> filhos, int* figurasInMemory) {

    std::vector<float> pontos;
    std::vector<float> pNormais;
    std::vector<float> pText;

    for (unsigned int subNodeAt = 0; (*figurasInMemory < numDeFiguras) && (subNodeAt < filhos.size()); subNodeAt++) {
        xmlData ficheiro = filhos.at(subNodeAt);
        std::vector<std::vector<triangulo>> figuraV = ficheiro.getPontos();
        std::vector<std::vector<triangulo>> fnmV = ficheiro.getNormais();
        std::vector<std::vector<triangulo>> ftxtV = ficheiro.getCoords();

        for (int pos = 0; pos < figuraV.size(); pos++) {
            std::vector<triangulo> figura = figuraV.at(pos);
            std::vector<triangulo> fnm = fnmV.at(pos);
            std::vector<triangulo> ftxt = ftxtV.at(pos);

            for (unsigned int k = 0; k < figura.size(); k++) {
                triangulo tr = figura.at(k);
                triangulo tr2 = fnm.at(k);
                triangulo tr3 = ftxt.at(k);

                pontos.push_back(tr.getVerticeX1());
                pontos.push_back(tr.getVerticeY1());
                pontos.push_back(tr.getVerticeZ1());
                pontos.push_back(tr.getVerticeX2());
                pontos.push_back(tr.getVerticeY2());
                pontos.push_back(tr.getVerticeZ2());
                pontos.push_back(tr.getVerticeX3());
                pontos.push_back(tr.getVerticeY3());
                pontos.push_back(tr.getVerticeZ3());

                pNormais.push_back(tr2.getVerticeX1());
                pNormais.push_back(tr2.getVerticeY1());
                pNormais.push_back(tr2.getVerticeZ1());
                pNormais.push_back(tr2.getVerticeX2());
                pNormais.push_back(tr2.getVerticeY2());
                pNormais.push_back(tr2.getVerticeZ2());
                pNormais.push_back(tr2.getVerticeX3());
                pNormais.push_back(tr2.getVerticeY3());
                pNormais.push_back(tr2.getVerticeZ3());

                pText.push_back(tr3.getVerticeX1());
                pText.push_back(tr3.getVerticeY1());
                pText.push_back(tr3.getVerticeX2());
                pText.push_back(tr3.getVerticeY2());
                pText.push_back(tr3.getVerticeX3());
                pText.push_back(tr3.getVerticeY3());
            }
            int size = pontos.size();
            int sizeN = pNormais.size();
            int sizeT = pText.size();
            tamanhoFiguras[*figurasInMemory] = size;
            glGenBuffers(1, &figuras[*figurasInMemory]);
            glBindBuffer(GL_ARRAY_BUFFER, figuras[*figurasInMemory]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pontos.size(), &pontos[0], GL_STATIC_DRAW);

            glGenBuffers(1, &normais[*figurasInMemory]);
            glBindBuffer(GL_ARRAY_BUFFER, normais[*figurasInMemory]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pNormais.size(), &pNormais[0], GL_STATIC_DRAW);

            glGenBuffers(1, &coordenadas[*figurasInMemory]);
            glBindBuffer(GL_ARRAY_BUFFER, coordenadas[*figurasInMemory]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pText.size(), &pText[0], GL_STATIC_DRAW);
            initTexture(ficheiro.getMateriais().at(0), *figurasInMemory);
            pontos.clear();
            pNormais.clear();
            pText.clear();
            *figurasInMemory += 1;
        }       
        toBuffers(ficheiro.getFilhos(), figurasInMemory);
    }
}

void toBuffers(std::vector<xmlData> figs) {
       
    numDeFiguras = 0;
    figurasDesenhadas = 0;
    
    getTotalFiguras(figs);

    globalInfo = figs;
    figuras = new GLuint[numDeFiguras];
    normais = new GLuint[numDeFiguras];;
    coordenadas = new GLuint[numDeFiguras];
    texture = new GLuint[numDeFiguras];
    tamanhoFiguras = new GLuint[numDeFiguras];
    std::vector<float> pontos;
    std::vector<float> pNormais;
    std::vector<float> pText;
    int figurasInMemory = 0;

    for (int mainNodeAt = 0; figurasInMemory < numDeFiguras; mainNodeAt++) {
        xmlData ficheiro = figs.at(mainNodeAt);
        std::vector<std::vector<triangulo>> figuraV = ficheiro.getPontos();
        std::vector<std::vector<triangulo>> fnmV = ficheiro.getNormais();
        std::vector<std::vector<triangulo>> ftxtV = ficheiro.getCoords();
        
        for(int pos = 0; pos < figuraV.size() ; pos++){
            std::vector<triangulo> figura = figuraV.at(pos);
            std::vector<triangulo> fnm = fnmV.at(pos);
            std::vector<triangulo> ftxt = ftxtV.at(pos);
        
            for (unsigned int k = 0; k < figura.size(); k++) {
                triangulo tr = figura.at(k);
                triangulo tr2 = fnm.at(k);
                triangulo tr3 = ftxt.at(k);

                pontos.push_back(tr.getVerticeX1());
                pontos.push_back(tr.getVerticeY1());
                pontos.push_back(tr.getVerticeZ1());
                pontos.push_back(tr.getVerticeX2());
                pontos.push_back(tr.getVerticeY2());
                pontos.push_back(tr.getVerticeZ2());
                pontos.push_back(tr.getVerticeX3());
                pontos.push_back(tr.getVerticeY3());
                pontos.push_back(tr.getVerticeZ3());

                pNormais.push_back(tr2.getVerticeX1());
                pNormais.push_back(tr2.getVerticeY1());
                pNormais.push_back(tr2.getVerticeZ1());
                pNormais.push_back(tr2.getVerticeX2());
                pNormais.push_back(tr2.getVerticeY2());
                pNormais.push_back(tr2.getVerticeZ2());
                pNormais.push_back(tr2.getVerticeX3());
                pNormais.push_back(tr2.getVerticeY3());
                pNormais.push_back(tr2.getVerticeZ3());

                pText.push_back(tr3.getVerticeX1());
                pText.push_back(tr3.getVerticeY1());
                pText.push_back(tr3.getVerticeX2());
                pText.push_back(tr3.getVerticeY2());
                pText.push_back(tr3.getVerticeX3());
                pText.push_back(tr3.getVerticeY3());
            }
        
            int size = pontos.size();
            int sizeN = pNormais.size();
            int sizeT = pText.size();
            tamanhoFiguras[figurasInMemory] = size;

            glGenBuffers(1, &figuras[figurasInMemory]);
            glBindBuffer(GL_ARRAY_BUFFER, figuras[figurasInMemory]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, &pontos[0], GL_STATIC_DRAW);
            glGenBuffers(1, &normais[figurasInMemory]);
            glBindBuffer(GL_ARRAY_BUFFER, normais[figurasInMemory]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sizeN, &pNormais[0], GL_STATIC_DRAW);
            glGenBuffers(1, &coordenadas[figurasInMemory]);
            glBindBuffer(GL_ARRAY_BUFFER, coordenadas[figurasInMemory]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sizeT, &pText[0], GL_STATIC_DRAW);

            initTexture(ficheiro.getMateriais().at(pos),figurasInMemory);
            pontos.clear();
            pNormais.clear();
            pText.clear();
            figurasInMemory +=1 ;
        }
        toBuffers(ficheiro.getFilhos(), &figurasInMemory);
    }
}

void startDrawing(int argc, char **argv, std::vector<xmlData> figs,std::vector<light> luz) {

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
    glutIdleFunc(renderScene);

// Callback registration for keyboard processing
    glutMouseFunc(processMouseButtons);
    glutMotionFunc(processMouseMotion);
    glutKeyboardFunc(processKeys);

//  OpenGL settings
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);

    glEnable(GL_LIGHTING);

    glEnable(GL_TEXTURE_2D);
   // glShadeModel(GL_SMOOTH);
    

// Funções de setup antes do main cycle
    toBuffers(figs);
    luzGlobal = luz;
    spherical2cartesian();

// enter GLUT's main cycle
    glutMainLoop();
}