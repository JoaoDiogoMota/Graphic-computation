#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>



float alfa = 0.0f, beta = 0.5f, radius = 100.0f;
float camX, camY, camZ;

int r = 50;
int ri = 35;
int rc = 15;
double somaAng = 0;


#define DEG2RAD 3.14159/180.0

void spherical2Cartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
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

// draw treeNumber trees randomly
void randomDraw(int treeNumber) {
	srand(1);
	
	for (int i = 0; i < treeNumber; i++) {
		float rx = static_cast <float> (rand() % 201 - 100); // random entre -100 e 100
		float ry = static_cast <float> (rand() % 201 - 100); // random entre -100 e 100
		float d1 = sqrt(pow(rx,2) + pow(ry,2));
		if (rx < 98 && rx > -98 && ry < 98 && ry > -98 && d1 >= 50) {
			glPushMatrix();
			glTranslatef(rx, 0, ry);
			drawTree();
			glPopMatrix();
		}
		else i--;
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

//Draws a ellpise made out of points.
void drawEye(float radiusX, float radiusY) {
	int i;
	int n = 8; 

	for (i = 0; i < n; i++) {
		float rad = i/n * 2 * M_PI;
		printf("%f %f \n", radiusX * sin(rad), radiusY * cos(rad));
	}

}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	//plano 
	/*glColor3f(0.2f, 0.8f, 0.2f);
	glBegin(GL_TRIANGLES);
		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);

		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);
		glVertex3f(100.0f, 0, 100.0f);
	glEnd();
	
	randomDraw(800);
	glColor3f(0.8, 0.2, 0.6);
	glutSolidTorus(1, 3, 20, 100);
	drawTeaPots();*/

//glutSolidSphere(1, 100, 100);
	drawEye(10,20);

	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here

}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 1.0f;
		if (radius < 1.0f)
			radius = 1.0f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 1.0f; break;
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


int main(int argc, char **argv) {

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
	//glEnable(GL_CULL_FACE);

	spherical2Cartesian();

	printInfo();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
