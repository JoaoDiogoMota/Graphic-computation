#include <IL/il.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>


#define _USE_MATH_DEFINES
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

float camX, camY, camZ;
float radius = 5;
float refX = 0, refY = 5, refZ = 0;
float alfa = 0.0f, beta = 0.5f, romm = 0.0f;

int r = 50;
int ri = 35;
int rc = 15;
double somaAng = 0;

int innitBoy = 0;

int startX, startY, tracking = 0;

unsigned int t, tw, th;
unsigned char* imageData;
GLuint buffers[1];
float* vertexB;

std::vector<float> trees;

void spherical2Cartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius;
	camZ = radius * cos(beta) * cos(alfa);
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

void drawTree() {
	// desenha tronco
	glPushMatrix();
	glRotatef(-90.0, 1, 0, 0);
	glColor3f(0.4, 0.2, 0);
	glutSolidCone(0.5, 4, 100, 10);
	glPopMatrix();

	//desenha corpo árvore
	glPushMatrix(); 
	glRotatef(-90.0, 1, 0, 0);
	glTranslatef(0, 0, 2);
	glColor3f(0.4, 0.6, 0.2);
	glutSolidCone(2, 7, 7, 7);
	glPopMatrix();
	 
}

float h(int i, int j) {
	float height = 0.12* imageData[i*tw + j];

	return height;
}

float hf(float x, float z) {
	float x1 = floor(x);
	float x2 = x1 + 1;
	float z1 = floor(z);
	float z2 = z1 + 1;
	float fz = z - z1;
	float fx = x - x1;
	float h_x1_z = h(z1,x1) * (1 - fz) + h(z1,x2) * fz;
	float h_x2_z = h(z2,x1) * (1 - fz) + h(z2,x2) * fz;
	float height_xz = h_x1_z * (1 - fx) + h_x2_z * fx;

	return height_xz;
}

// draw treeNumber trees randomly
void randomDraw(int treeNumber) {
	srand(1);
	
	for (int i = 0; i < treeNumber; i++) {
		float rx = static_cast <float> (rand() % 220 - 100); // random entre -100 e 100
		float ry = static_cast <float> (rand() % 220 - 100); // random entre -100 e 100
		float d1 = sqrt(pow(rx,2) + pow(ry,2));
		float rkk = hf(rx, ry);
		if (rx < 98 && rx > -98 && ry < 98 && ry > -98 && d1 >= 50) {
			trees.push_back(rx);
			trees.push_back(rkk);
			trees.push_back(ry);
		}
		else i--;
	}

}

void theActualTreeDraw() {
	for (int pos = 0; pos < trees.size(); pos += 3) {
		glPushMatrix();
		glTranslatef(trees.at(pos),trees.at(pos+1),trees.at(pos+2));
		drawTree();
		glPopMatrix();
	}
}


void drawTeaPotsIn(int num, int raio) {
	int i = 0;
	float ang = (2 * M_PI) / num;
	float roda = 360 / num;
	float teapotsize = 1.5;
	GLdouble nx;
	GLdouble nz;
	while (i < num) {
		float nang = i * ang;
		nx = cos(nang + somaAng) * raio;
		nz = sin(nang + somaAng) * raio;
		glPushMatrix();
		glColor3f(0.0f, 0.0f, 0.7f);
		glTranslatef(nx, teapotsize, nz);
		glRotatef(-1 * (roda * i + ((180 * somaAng) / M_PI)), 0, 1, 0);
		glutSolidTeapot(teapotsize);
		glPopMatrix();
		i++;
	}
}

void drawTeaPotsOut(int num, int raio) {
	int i = 0;
	float ang = (2 * M_PI) / num;
	float roda = 360 / num;
	float teapotsize = 1.5;
	while (i < num) {
		float nang = i * ang;
		GLdouble nx = cos(nang - somaAng) * raio;
		GLdouble nz = sin(nang - somaAng) * raio;
		glPushMatrix();
		glColor3f(0.7f, 0.0f, 0.0f);
		glTranslatef(nx, teapotsize, nz);
		glRotatef(-1 * (roda * i - ((180 * somaAng) / M_PI) - 90), 0, 1, 0);
		glutSolidTeapot(teapotsize);
		glPopMatrix();
		i++;
	}
}

void drawTeaPots() {
	drawTeaPotsIn(8, 15);
	drawTeaPotsOut(20, 35);
	somaAng += 0.03;
	glutPostRedisplay();
}

void drawTerrain() {

	ilGenImages(1, &t);
	ilBindImage(t);
	ilLoadImage((ILstring)"terreno.jpg");
	ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);

	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);

	std::cout << "Width:" << tw << " | Height:" << th << std::endl;

	imageData = ilGetData();

	float halfw = tw / 2;
	float halfh = th / 2;

	int pontos = 2 * 3 * tw * th;

	float* vertexB = new float[pontos];
	int indice = 0;

	for (int i = 0; i < th - 1; i++) {
		for (int j = 0; j < tw; j++) {
			vertexB[indice++] = j - halfw;
			vertexB[indice++] = h(i, j);
			vertexB[indice++] = i - halfh;
			vertexB[indice++] = j - halfw;
			vertexB[indice++] = h(i+1, j);
			vertexB[indice++] = (i + 1) - halfh;
		}
	}

	glGenBuffers(1, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, pontos * sizeof(float), vertexB, GL_STATIC_DRAW);
}

void renderScene(void) {

	float pos[4] = { -1.0, 1.0, 1.0, 0.0 };

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX+refX, camY+ hf(refZ,refX) , camZ+refZ ,
			  refX, refY , refZ,
		      0.0f, 1.0f, 0.0f);


	//plano 
	/*
	glColor3f(0.2f, 0.8f, 0.2f);
	glBegin(GL_TRIANGLES);
		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);

		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);
		glVertex3f(100.0f, 0, 100.0f);
	glEnd();
	*/
	if (innitBoy == 0) {
		randomDraw(300);
		innitBoy = 1;
	}
	theActualTreeDraw();
	glColor3f(0.8, 0.2, 0.6);
	glutSolidTorus(1, 3, 20, 100);
	drawTeaPots();

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	
	glColor3f(0.5f,0.35f,0.05f);
	for (int i = 0; i < th - 1; i++) {
		glDrawArrays(GL_LINE_STRIP, (tw) * 2 * i, (tw) * 2);
	}

	// End of frame
	glutSwapBuffers();
}

void processMouseButtons(int button, int state, int xx, int yy) {
}


void processMouseMotion(int xx, int yy) {
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
		alfa -= 0.04; break;
	case(GLUT_KEY_LEFT):
		alfa += 0.04; break;
	case(GLUT_KEY_UP):
		refY += 0.1;
		if (beta > 1.5f)
			beta = 1.5f;
		break;
	case(GLUT_KEY_DOWN):
		refY -= 0.1;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 5.0f;
		if (radius < 1.0f)
			radius = 1.0f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 5.0f; break;
	}
	
	spherical2Cartesian();
	glutPostRedisplay();
}


void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Home and End control the distance from the camera to the origin");
}

void init() {

	glEnableClientState(GL_VERTEX_ARRAY);
	// 	Load the height map "terreno.jpg"
	glewInit();
	ilInit();

	// 	Build the vertex arrays
	drawTerrain();
	// 	OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
}

int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

	init();

	spherical2Cartesian();

	printInfo();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
