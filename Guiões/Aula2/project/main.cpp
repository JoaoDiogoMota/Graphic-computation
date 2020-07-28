#ifdef __APPLE__
#include <GL/glew.h> 
#include <GLUT/glut.h>
#else
#include <GL/glew.h> 
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

float camx, camy, camz;
float alfa = 0.0f;
float beta = 0.5f;
float radius = 9.0f;

GLuint buffers[1];
int size = 0;
int check = 0;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
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
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void spherical2cartesian() {
	camx = radius * sin(alfa) * cos(beta);
	camz = radius * cos(alfa) * cos(beta);
	camy = radius * sin(beta);
}

void drawCylinder(float radius, float height, int sides) {

	int i;
	float step;

	std::vector<float> vertexes;

	step = 360.0 / sides;

	// top
	for (i = 0; i < sides; i++) {
		vertexes.push_back(0); vertexes.push_back(height * 0.5); vertexes.push_back(0);
		vertexes.push_back(cos(i * step * M_PI / 180.0) * radius); vertexes.push_back(height * 0.5); vertexes.push_back(-sin(i * step * M_PI / 180.0) * radius);
		vertexes.push_back(cos((i + 1) * step * M_PI / 180.0) * radius); vertexes.push_back(height * 0.5); vertexes.push_back(-sin((i + 1) * step * M_PI / 180.0) * radius);
	}

	// bottom
	for (i = 0; i < sides; i++) {
		vertexes.push_back(0); vertexes.push_back(-height * 0.5); vertexes.push_back(0);
		vertexes.push_back(cos((i + 1) * step * M_PI / 180.0) * radius); vertexes.push_back(-height * 0.5); vertexes.push_back(-sin((i + 1) * step * M_PI / 180.0) * radius);
		vertexes.push_back(cos(i * step * M_PI / 180.0) * radius); vertexes.push_back(-height * 0.5); vertexes.push_back(-sin(i * step * M_PI / 180.0) * radius);
	}

	// body
	for (i = 0; i <= sides; i++) {

		vertexes.push_back(cos(i * step * M_PI / 180.0) * radius); vertexes.push_back(height * 0.5); vertexes.push_back(-sin(i * step * M_PI / 180.0) * radius);
		vertexes.push_back(cos(i * step * M_PI / 180.0) * radius); vertexes.push_back(-height * 0.5); vertexes.push_back(-sin(i * step * M_PI / 180.0) * radius);
		vertexes.push_back(cos((i + 1) * step * M_PI / 180.0) * radius); vertexes.push_back(height * 0.5); vertexes.push_back(-sin((i + 1) * step * M_PI / 180.0) * radius);

		vertexes.push_back(cos(i * step * M_PI / 180.0) * radius); vertexes.push_back(-height * 0.5); vertexes.push_back(-sin(i * step * M_PI / 180.0) * radius);
		vertexes.push_back(cos((i + 1) * step * M_PI / 180.0) * radius); vertexes.push_back(-height * 0.5); vertexes.push_back(-sin((i + 1) * step * M_PI / 180.0) * radius);
		vertexes.push_back(cos((i + 1) * step * M_PI / 180.0) * radius); vertexes.push_back(height * 0.5); vertexes.push_back(-sin((i + 1) * step * M_PI / 180.0) * radius);
	}

	size = vertexes.size();
	glGenBuffers(1, &buffers[0]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexes.size(), &vertexes[0], GL_STATIC_DRAW);
	vertexes.clear();
}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camx, camy, camz,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);
	if (check == 0) {
		buffers = new GLuint[1];
		cylinder(1, 2, 10);
		check++;
	}

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, size);

	int time; 

	frame++; 
	time = glutGet(GLUT_ELAPSED_TIME); 
	if (time - timebase > 1000) { 
		fps = frame * 1000.0 / (time - timebase)); 
		timebase = time; 
		frame = 0; 
	}

	glutSetWindowTitle(time);

	drawCylinder(1, 2, 30);

	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

	// put code to process regular keys in here

}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {
	case (GLUT_KEY_UP):
		beta += 0.1f; break;
		if (beta > 1.5) beta = 1.5f; break;
	case (GLUT_KEY_DOWN):
		beta -= 0.1f; break;
		if (beta < -1.5) beta = -1.5f; break;
	case (GLUT_KEY_LEFT):
		alfa += 0.1f; break;
	case (GLUT_KEY_RIGHT):
		alfa -= 0.1f; break;
	}
	spherical2cartesian();
	glutPostRedisplay();
}


int main(int argc, char** argv) {

	glewInit();
	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("CG@DI-UM");
	glEnableClientState(GL_VERTEX_ARRAY);
	glutSetWindowTitle("fps");

	// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}
