#ifdef _WIN32
#include <windows.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

float prevY[3] = { 0,1,0 };
int** indexes;
int patches;
float* pontosX;
float* pontosY;
float* pontosZ;
#define SIZE_BUFFER 1024

void drawPlan(float comp,std::string name) {
	ofstream myfile;
	myfile.open("../../SistemaSolar/"+name);

	float c = comp / 2;

	// ESQUERDO
	myfile << c << " " << "0.0" << " " << '-' << c << '\n';
	myfile << '-' << c << " " << "0.0" << " " << c << '\n';
	myfile << c << " " << "0.0" << " " << c << '\n';

	// DIREITO
	myfile << c << " " << "0.0" << " " << '-' << c << '\n';
	myfile << '-' << c << " " << "0.0" << " " << '-' << c << '\n';
	myfile << '-' << c << " " << "0.0" << " " << c << '\n';

	myfile.close();

}

void multMatrixVector(float* m, float* v, float* res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}

}

void multVectorMatrix(float* v, float* m, float* res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}
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

/* Function that draws a Box given the X,Y and Z dimensions (and the number of divisions)*/
void drawBox(float xDimension, float yDimension, float zDimension, int divisions,std::string name) {
	ofstream myfile;
	myfile.open("../../SistemaSolar/"+name);


	float xA = xDimension / 2;
	float yA = yDimension / 2;
	float zA = zDimension / 2;

	float sliceX = xDimension / divisions;
	float sliceY = yDimension / divisions;
	float sliceZ = zDimension / divisions;

	for (int i = 0; i < divisions; i++) {
		for (int j = 0; j < divisions; j++) {

			float ni = (float)i;
			float nj = (float)j;

			//face superior
			myfile << (-xA + (nj * sliceX)) << " " << yA << " " << (-zA + (ni * sliceZ)) << '\n';
			myfile << (-xA + (nj * sliceX)) << " " << yA << " " << (-zA + sliceZ + (ni * sliceZ)) << '\n';
			myfile << (-xA + sliceX + (nj * sliceX)) << " " << yA << " " << (-zA + sliceZ + (ni * sliceZ)) << '\n';

			myfile << (-xA + sliceX + (sliceX * nj)) << " " << yA << " " << (-zA + (sliceZ * ni)) << '\n';
			myfile << (-xA + (nj * sliceX)) << " " << yA << " " << (-zA + (ni * sliceZ)) << '\n';
			myfile << (-xA + sliceX + (sliceX * nj)) << " " << yA << " " << (-zA + sliceZ + (sliceZ * ni)) << '\n';

			//face inferior
			myfile << (-xA + sliceX + (nj * sliceX)) << " " << -yA << " " << (-zA + (ni * sliceZ)) << '\n';
			myfile << (-xA + sliceX + (nj * sliceX)) << " " << -yA << " " << (-zA + sliceZ + (ni * sliceZ)) << '\n';
			myfile << (-xA + (sliceX * nj)) << " " << -yA << " " << (-zA + sliceZ + (sliceZ * ni)) << '\n';


			myfile << (-xA + (sliceX * nj)) << " " << -yA << " " << (-zA + (sliceZ * ni)) << '\n';
			myfile << (-xA + sliceX + (nj * sliceX)) << " " << -yA << " " << (-zA + (ni * sliceZ)) << '\n';
			myfile << (-xA + (sliceX * nj)) << " " << -yA << " " << (-zA + sliceZ + (sliceZ * ni)) << '\n';

			//face frontal
			myfile << (-xA + (nj * sliceX)) << " " << (-yA + (ni * sliceY)) << " " << zA << '\n';
			myfile << (-xA + sliceX + (nj * sliceX)) << " " << (-yA + (ni * sliceY)) << " " << zA << '\n';
			myfile << (-xA + (sliceX * nj)) << " " << (-yA + sliceY + (ni * sliceY)) << " " << zA << '\n';

			myfile << (-xA + (sliceX * nj)) << " " << (-yA + sliceY + (sliceY * ni)) << " " << zA << '\n';
			myfile << (-xA + sliceX + (nj * sliceX)) << " " << (-yA + (ni * sliceY)) << " " << zA << '\n';
			myfile << (-xA + sliceX + (sliceX * nj)) << " " << (-yA + sliceY + (ni * sliceY)) << " " << zA << '\n';

			//face traseira
			myfile << (-xA + (sliceX * nj)) << " " << (-yA + (ni * sliceY)) << " " << -zA << '\n';
			myfile << (-xA + (nj * sliceX)) << " " << (-yA + sliceY + (ni * sliceY)) << " " << -zA << '\n';
			myfile << (-xA + sliceX + (sliceX * nj)) << " " << (-yA + sliceY + (ni * sliceY)) << " " << -zA << '\n';

			myfile << (-xA + sliceX + (sliceX * nj)) << " " << (-yA + (ni * sliceY)) << " " << -zA << '\n';
			myfile << (-xA + (nj * sliceX)) << " " << (-yA + (ni * sliceY)) << " " << -zA << '\n';
			myfile << (-xA + sliceX + (sliceX * nj)) << " " << (-yA + sliceY + (ni * sliceY)) << " " << -zA << '\n';

			//face lateral esquerda
			myfile << -xA << " " << (-yA + (nj * sliceY)) << " " << (-zA + sliceZ + (ni * sliceZ)) << '\n';
			myfile << -xA << " " << (-yA + sliceY + (nj * sliceY)) << " " << (-zA + sliceZ + (ni * sliceZ)) << '\n';
			myfile << -xA << " " << (-yA + sliceY + (nj * sliceY)) << " " << (-zA + (ni * sliceZ)) << '\n';

			myfile << -xA << " " << (-yA + (nj * sliceY)) << " " << (-zA + (ni * sliceZ)) << '\n';
			myfile << -xA << " " << (-yA + (nj * sliceY)) << " " << (-zA + sliceZ + (ni * sliceZ)) << '\n';
			myfile << -xA << " " << (-yA + sliceY + (nj * sliceY)) << " " << (-zA + (ni * sliceZ)) << '\n';

			//face lateral direita
			myfile << xA << " " << (-yA + (nj * sliceY)) << " " << (-zA + (ni * sliceZ)) << '\n';
			myfile << xA << " " << (-yA + sliceY + (nj * sliceY)) << " " << (-zA + (ni * sliceZ)) << '\n';
			myfile << xA << " " << (-yA + sliceY + (nj * sliceY)) << " " << (-zA + sliceZ + (ni * sliceZ)) << '\n';

			myfile << xA << " " << (-yA + (nj * sliceY)) << " " << (-zA + sliceZ + (ni * sliceZ)) << '\n';
			myfile << xA << " " << (-yA + (nj * sliceY)) << " " << (-zA + (ni * sliceZ)) << '\n';
			myfile << xA << " " << (-yA + sliceY + (nj * sliceY)) << " " << (-zA + sliceZ + (ni * sliceZ)) << '\n';

		}
	}
	myfile.close();
}

void drawEsfera(float radius, float stacks, float slices,std::string name) {
	float sliceStep = (float) (2 * M_PI) / slices;
	float stackStep = (float) M_PI / stacks;
	float x, y, z;
	float x2, y2, z2;
	float x3, y3, z3;
	float x4, y4, z4;
	float sliceAngle, stackAngle;
	ofstream myfile;
	myfile.open("../../SistemaSolar/"+name);

	for (int i = 0; i < stacks + 1; i++) {
		stackAngle = M_PI / 2 - ((double)i * stackStep); // starting from -pi to pi

		for (int j = 0; j <= slices; ++j)
		{
			sliceAngle = j * sliceStep; // starting from 0 to 2pi

			//Ponto 1
			z = radius * cosf(stackAngle) * cosf(sliceAngle);	// r * cos(u) * cos(v)
			x = radius * cosf(stackAngle) * sinf(sliceAngle);	// r * cos(u) * sin(v)
			y = radius * sinf(stackAngle); // r * sin(v)

			//Ponto 2
			z2 = radius * cosf(stackAngle) * cosf(sliceAngle + sliceStep);	// r * cos(u) * cos(v)
			x2 = radius * cosf(stackAngle) * sinf(sliceAngle + sliceStep);	// r * cos(u) * sin(v)
			y2 = y;

			//Ponto 3
			z3 = radius * cosf(stackAngle + stackStep) * cosf(sliceAngle);	// r * cos(u) * cos(v)
			x3 = radius * cosf(stackAngle + stackStep) * sinf(sliceAngle);	// r * cos(u) * sin(v)
			y3 = radius * sinf(stackAngle + stackStep); // r * sin(v)

			//Ponto 4
			z4 = radius * cosf(stackAngle + stackStep) * cosf(sliceAngle + sliceStep);	// r * cos(u) * cos(v)
			x4 = radius * cosf(stackAngle + stackStep) * sinf(sliceAngle + sliceStep);	// r * cos(u) * sin(v)
			y4 = radius * sinf(stackAngle + stackStep); // r * sin(v)

			myfile << x << " " << y << " " << z << '\n';
			myfile << x2 << " " << y2 << " " << z2 << '\n';
			myfile << x3 << " " << y3 << " " << z3 << '\n';

			if (i > 0) {
				myfile << x3 << " " << y3 << " " << z3 << '\n';
				myfile << x2 << " " << y2 << " " << z2 << '\n';
				myfile << x4 << " " << y4 << " " << z4 << '\n';
			}
		}
	}
	myfile.close();
}

void drawCone(float radius, float height, float slices, float stacks,std::string name) {
	ofstream myfile;
	myfile.open("../../SistemaSolar/"+name);

	float angle = ((2 * M_PI) / slices);
	float stackDiv = height / stacks;
	float stackRaio = radius / stacks;
	float totAlt = 0, totRaio = 0;

	for (int i = 0; i < stacks; i++) {
		totRaio = radius - (stackRaio * i);
		totAlt = stackDiv * i;

		for (int j = 0; j < slices; j++) {
			float x = totRaio * sin(angle * j);
			float z = totRaio * cos(angle * j);
			float y = totAlt;

			float x1 = totRaio * sin((j + 1) * angle);
			float z1 = totRaio * cos((j + 1) * angle);

			if (totAlt == 0) {
				myfile << "0.0" << " " << y << " " << "0.0" << '\n';
				myfile << x1 << " " << y << " " << z1 << '\n';
				myfile << x << " " << y << " " << z << '\n';
			}

			myfile << x << " " << y << " " << z << '\n';
			myfile << x1 << " " << y << " " << z1 << '\n';
			myfile << (totRaio - stackRaio) * sin((j + 1) * angle) << " " << (y + stackDiv) << " " << (totRaio - stackRaio) * cos((j + 1) * angle) << '\n';

			if (i < stacks - 1) {
				myfile << x << " " << y << " " << z << '\n';
				myfile << (totRaio - stackRaio) * sin((j + 1) * angle) << " " << (y + stackDiv) << " " << (totRaio - stackRaio) * cos((j + 1) * angle) << '\n';
				myfile << (totRaio - stackRaio) * sin(j * angle) << " " << (y + stackDiv) << " " << (totRaio - stackRaio) * cos(j * angle) << '\n';
			}
		}
	}
	myfile.close();
}

void drawArc(float outradius, float inradius, int slices, std::string name) {
	ofstream myfile;
	myfile.open("../../SistemaSolar/" + name);

	float sliceStep = (float)(2 * M_PI) / slices;

	for (int i = 0; i <= slices; i++) {

		myfile << inradius * cos(i * sliceStep) << " " << "0.0" << " " << inradius * sin(i * sliceStep) << '\n';
		myfile << outradius * cos((i + 1) * sliceStep) << " " << "0.0" << " " << outradius * sin((i + 1) * sliceStep) << '\n';
		myfile << outradius * cos(i * sliceStep) << " " << "0.0" << " " << outradius * sin(i * sliceStep) << '\n';

		myfile << outradius * cos((i + 1) * sliceStep) << " " << "0.0" << " " << outradius * sin((i + 1) * sliceStep) << '\n';
		myfile << inradius * cos(i * sliceStep) << " " << "0.0" << " " << inradius * sin(i * sliceStep) << '\n';
		myfile << inradius * cos((i + 1) * sliceStep) << " " << "0.0" << " " << inradius * sin((i + 1) * sliceStep) << '\n';

		myfile << inradius * cos(i * sliceStep) << " " << "0.0" << " " << inradius * sin(i * sliceStep) << '\n';
		myfile << outradius * cos(i * sliceStep) << " " << "0.0" << " " << outradius * sin(i * sliceStep) << '\n';
		myfile << outradius * cos((i + 1) * sliceStep) << " " << "0.0" << " " << outradius * sin((i + 1) * sliceStep) << '\n';		

		myfile << outradius * cos((i + 1) * sliceStep) << " " << "0.0" << " " << outradius * sin((i + 1) * sliceStep) << '\n';
		myfile << inradius * cos((i + 1) * sliceStep) << " " << "0.0" << " " << inradius * sin((i + 1) * sliceStep) << '\n';
		myfile << inradius * cos(i * sliceStep) << " " << "0.0" << " " << inradius * sin(i * sliceStep) << '\n';
	}
	myfile.close();

}

void drawBelt(float inradius, float outradius, int slices, int stacks, std::string name) {

	ofstream myfile;
	myfile.open("../../SistemaSolar/" + name);


	for (int i = 0; i < stacks; i++) {
		float stackAngle = i * ((2 * M_PI) / stacks);
		float stackAngle1 = (i + 1) * ((2 * M_PI) / stacks);
		for (int j = 0; j < slices; j++) {
			float sliceAngle = j * ((2 * M_PI) / slices);
			float sliceAngle1 = (j + 1) * ((2 * M_PI) / slices);

			myfile << cos(stackAngle) * (inradius * cos(sliceAngle1) + outradius) << " " << sin(stackAngle) * (inradius * cos(sliceAngle1) + outradius) << " " << inradius * sin(sliceAngle1) << '\n';
			myfile << cos(stackAngle) * (inradius * cos(sliceAngle) + outradius) << " " << sin(stackAngle) * (inradius * cos(sliceAngle) + outradius) << " " << inradius * sin(sliceAngle) << '\n';
			myfile << cos(stackAngle1) * (inradius * cos(sliceAngle1) + outradius) << " " << sin(stackAngle1) * (inradius * cos(sliceAngle1) + outradius) << " " << inradius * sin(sliceAngle1) << '\n';
			
			myfile << cos(stackAngle) * (inradius * cos(sliceAngle) + outradius) << " " << sin(stackAngle) * (inradius * cos(sliceAngle) + outradius) << " " << inradius * sin(sliceAngle) << '\n';
			myfile << cos(stackAngle1) * (inradius * cos(sliceAngle) + outradius) << " " << sin(stackAngle1) * (inradius * cos(sliceAngle) + outradius) << " " << inradius * sin(sliceAngle) << '\n';
			myfile << cos(stackAngle1) * (inradius * cos(sliceAngle1) + outradius) << " " << sin(stackAngle1) * (inradius * cos(sliceAngle1) + outradius) << " " << inradius * sin(sliceAngle1) << '\n';
		}
	}
	myfile.close();
}

int parserBezier(std::string patch) {
	char line[SIZE_BUFFER];
	int j;
	int nPontos;
	int nPatches;
	std::string final = "../../SistemaSolar/" + patch;
	FILE* f = fopen(final.c_str(), "r");
	if (!f) {
		printf("file not opened\n");
		return 1;
	}
	fscanf(f, "%d\n", &nPatches);
	patches = nPatches;
	//printf("%d\n", patches);

	indexes = (int**)malloc(sizeof(int*) * patches);
	for (int i = 0; i < patches; i++) {
		indexes[i] = (int*)malloc(sizeof(int) * 16);
		memset(line, 0, SIZE_BUFFER);
		fgets(line, SIZE_BUFFER, f);
		char* token = NULL;
		for (j = 0, token = strtok(line, ", "); token && j < 16; token = strtok(NULL, ", "), j++) {
			indexes[i][j] = atoi(token);
		}
	}

	fscanf(f, "%d\n", &nPontos);
	pontosX = (float*)malloc(sizeof(float) * nPontos);
	pontosY = (float*)malloc(sizeof(float) * nPontos);
	pontosZ = (float*)malloc(sizeof(float) * nPontos);

	for (int k = 0; k < nPontos; k++) {
		memset(line, 0, SIZE_BUFFER);
		fgets(line, SIZE_BUFFER, f);
		pontosX[k] = atof(strtok(line, ", "));
		pontosY[k] = atof(strtok(NULL, ", "));
		pontosZ[k] = atof(strtok(NULL, ", "));
	}
	fclose(f);
	//printf("%f\n", pontosX[6]);
	return 0;

}

void getBezierPoint(float u, float v, float* px, float* py, float* pz, float* normalV) {

	float m[4][4] = { {-1.0f,  3.0f, -3.0f,  1.0f},
					  { 3.0f, -6.0f,  3.0f,  0.0f},
					  {-3.0f,  3.0f,  0.0f,  0.0f},
					  { 1.0f,  0.0f,  0.0f,  0.0f}
	};

	float Px[4][4] = { {px[0],  px[1],  px[2],  px[3] },
					   {px[4],  px[5],  px[6],  px[7] },
					   {px[8],  px[9],  px[10], px[11]},
					   {px[12], px[13], px[14], px[15]}
	};

	float Py[4][4] = { {py[0],  py[1],  py[2],  py[3] },
					   {py[4],  py[5],  py[6],  py[7] },
					   {py[8],  py[9],  py[10], py[11]},
					   {py[12], py[13], py[14], py[15]}
	};

	float Pz[4][4] = { {pz[0],  pz[1],  pz[2],  pz[3] },
					   {pz[4],  pz[5],  pz[6],  pz[7] },
					   {pz[8],  pz[9],  pz[10], pz[11]},
					   {pz[12], pz[13], pz[14], pz[15]}
	};

	float U[4] = { u * u * u , u * u , u, 1 };
	float Ud[4] = { 3 * u * u , 2 * u, 1, 0 };

	float V[4] = { v * v * v , v * v , v, 1 };
	float Vd[4] = { 3 * v * v , 2 * v, 1, 0 };

	float UM[4];
	multVectorMatrix(U, (float*)m, UM);
	float UMd[4];
	multVectorMatrix(Ud, (float*)m, UMd);

	float MV[4];
	multMatrixVector((float*)m, V, MV);
	float MVd[4];
	multMatrixVector((float*)m, Vd, MVd);

	float UMP[3][4];
	multVectorMatrix(UM, (float*)Px, UMP[0]);
	multVectorMatrix(UM, (float*)Py, UMP[1]);
	multVectorMatrix(UM, (float*)Pz, UMP[2]);

	float UMPd[3][4];
	multVectorMatrix(UMd, (float*)Px, UMPd[0]);
	multVectorMatrix(UMd, (float*)Py, UMPd[1]);
	multVectorMatrix(UMd, (float*)Pz, UMPd[2]);

	float derivU[3];
	float derivV[3];

	for (int i = 0; i < 3; i++) {
		normalV[i] = 0.0f;
		derivU[i] = 0.0f;
		derivV[i] = 0.0f;

		for (int j = 0; j < 4; j++) {
			normalV[i] += MV[j] * UMP[i][j];
			derivU[i] += UMPd[i][j] * MV[j];
			derivV[i] += UMP[i][j] * MVd[j];
		}
	}

	normalize(derivU);
	normalize(derivV);
	//cross(derivU, derivV,normalV);
	//normalize(normalV);
}

int genBezier(std::string name, int tesselation) {
	std::ofstream myfile;
	myfile.open("../../SistemaSolar/" + name);
	float arrX[16];
	float arrY[16];
	float arrZ[16];

	for (int i = 0; i < patches; i++) {
		for (int j = 0; j < 16; j++) {
			arrX[j] = pontosX[indexes[i][j]];
			arrY[j] = pontosY[indexes[i][j]];
			arrZ[j] = pontosZ[indexes[i][j]];
		}
		for (int u = 0; u < tesselation; u++) {
			float p0[3];
			float p1[3];
			float p2[3];
			float p3[3];
			for (int v = 0; v < tesselation; v++) {
				getBezierPoint(u / (float)tesselation, v / (float)tesselation, arrX, arrY, arrZ, p0);
				getBezierPoint((u + 1) / (float)tesselation, v / (float)tesselation, arrX, arrY, arrZ, p1);
				getBezierPoint(u / (float)tesselation, (v + 1) / (float)tesselation, arrX, arrY, arrZ, p2);
				getBezierPoint((u + 1) / (float)tesselation, (v + 1) / (float)tesselation, arrX, arrY, arrZ, p3);
/*
				glBegin(GL_TRIANGLES);
					glColor3f(1.0, 0.0, 0.0);
					glVertex3f(p0[0], p0[1], p0[2]);
					glVertex3f(p3[0], p3[1], p3[2]);
					glVertex3f(p2[0], p2[1], p2[2]);
					glColor3f(0.0, 0.0, 1.0);
					glVertex3f(p1[0], p1[1], p1[2]);
					glVertex3f(p3[0], p3[1], p3[2]);
					glVertex3f(p0[0], p0[1], p0[2]);
				glEnd();

*/
				myfile << p0[0] << " " << p0[1] << " " << p0[2] << '\n';
				myfile << p3[0] << " " << p3[1] << " " << p3[2] << '\n';
				myfile << p2[0] << " " << p2[1] << " " << p2[2] << '\n';

				myfile << p1[0] << " " << p1[1] << " " << p1[2] << '\n';
				myfile << p3[0] << " " << p3[1] << " " << p3[2] << '\n';
				myfile << p0[0] << " " << p0[1] << " " << p0[2] << '\n';
			}
		}
	}
	myfile.close();
	return 0;
}

/*
---------- COMANDOS PARA TERMINAL ----------

	-figura- <------valores------>

	<----->
	cinto raiointerno raioexterno slices stacks filename (cinto 1.5 2 50 50 cinto.3d)
	<----->
	arco raioexterno raiointerno slices filename (arco 2 1.5 50 arco.3d)
	<----->
	cone raio altura slices stacks filename (cone 2 3 20 20 cone.3d)
	<----->
	esfera raio stacks slices filename (esfera 2 30 30 esfera.3d)
	<----->
	caixa comprimento altura largura divisoes filename (caixa 2 2 2 5 box.3d)
	<----->
	plano comprimento_lado filename (plano 5 plano.3d)

----------<--------<---->-------->----------
*/

int main(int argc, char **argv){

	std::string argument(argv[1]);

	if (argument == "cone") {
		std::string raio(argv[2]);
		std::string altura(argv[3]);
		std::string slices(argv[4]);
		std::string stacks(argv[5]);
		std::string name(argv[6]);

		drawCone(std::stof(raio),std::stof(altura),std::stof(slices),std::stof(stacks),name);
	}
	if (argument == "arco") {
		std::string raioexterno(argv[2]);
		std::string raiointerno(argv[3]);
		std::string slices(argv[4]);
		std::string name(argv[5]);

		drawArc(std::stof(raioexterno), std::stof(raiointerno), std::stof(slices),name);
	}
	if (argument == "cinto") {
		std::string raiointerno(argv[2]);
		std::string raioexterno(argv[3]);
		std::string slices(argv[4]);
		std::string stacks(argv[5]);
		std::string name(argv[6]);

		drawBelt(std::stof(raiointerno), std::stof(raioexterno), std::stof(slices), std::stof(stacks), name);
	}
	if (argument == "esfera") {
		std::string raio(argv[2]);
		std::string stacks(argv[3]);
		std::string slices(argv[4]);
		std::string name(argv[5]);

		drawEsfera(std::stof(raio), std::stof(stacks), std::stof(slices),name);
	}
	if (argument == "caixa") {
		std::string comprimento(argv[2]);
		std::string altura(argv[3]);
		std::string largura(argv[4]);
		std::string divs(argv[5]);
		std::string name(argv[6]);

		drawBox(std::stof(comprimento), std::stof(altura), std::stof(largura), std::stof(divs),name);
	}
	if (argument == "plano") {
		std::string comprimento(argv[2]);
		std::string name(argv[3]);

		drawPlan(std::stof(comprimento),name);
	}
	if (argument == "bezier") {
		std::string patch(argv[2]);
		std::string name(argv[3]);
		std::string tesselation(argv[4]);
		parserBezier(patch);
		genBezier(name, std::atoi(tesselation.c_str()));
	}

	return 1;
}
