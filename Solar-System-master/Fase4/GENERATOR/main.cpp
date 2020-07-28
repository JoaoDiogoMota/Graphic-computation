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

void drawPlan(float comp, std::string name) {
	ofstream myfile;
	myfile.open("../../SistemaSolar/" + name);

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

void normaisPlan(float comp, std::string name) {
	ofstream myfile;
	myfile.open("../../SistemaSolar/" + name);

	// ESQUERDO
	myfile << "0.0" << " " << "1.0" << " " << "0.0" << '\n';
	myfile << "0.0" << " " << "1.0" << " " << "0.0" << '\n';
	myfile << "0.0" << " " << "1.0" << " " << "0.0" << '\n';

	// DIREITO
	myfile << "0.0" << " " << "1.0" << " " << "0.0" << '\n';
	myfile << "0.0" << " " << "1.0" << " " << "0.0" << '\n';
	myfile << "0.0" << " " << "1.0" << " " << "0.0" << '\n';

	myfile.close();

}

void texturePlan(float comp, std::string name) {
	ofstream myfile;
	myfile.open("../../SistemaSolar/" + name);

	//Esquerdo
	myfile << "1.0" << " " << "1.0" << '\n';
	myfile << "0.0" << " " << "0.0" << '\n';
	myfile << "1.0" << " " << "0.0" << '\n';
	//Direito
	myfile << "1.0" << " " << "1.0" << '\n';
	myfile << "0.0" << " " << "1.0" << '\n';
	myfile << "0.0" << " " << "0.0" << '\n';

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
void drawBox(float xDimension, float yDimension, float zDimension, int divisions, std::string name) {
	ofstream myfile;
	myfile.open("../../SistemaSolar/" + name);


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

void normaisBox(float xDimension, float yDimension, float zDimension, int divisions, std::string name) {
	ofstream myfile;
	myfile.open("../../SistemaSolar/" + name);

	for (int i = 0; i < divisions; i++) {
		for (int j = 0; j < divisions; j++) {
			//face superior
			myfile << "0.0" << " " << "1.0" << " " << "0.0" << '\n';
			myfile << "0.0" << " " << "1.0" << " " << "0.0" << '\n';
			myfile << "0.0" << " " << "1.0" << " " << "0.0" << '\n';
			myfile << "0.0" << " " << "1.0" << " " << "0.0" << '\n';
			myfile << "0.0" << " " << "1.0" << " " << "0.0" << '\n';
			myfile << "0.0" << " " << "1.0" << " " << "0.0" << '\n';
			//face inferior
			myfile << "0.0" << " " << '-' << "1.0" << " " << "0.0" << '\n';
			myfile << "0.0" << " " << '-' << "1.0" << " " << "0.0" << '\n';
			myfile << "0.0" << " " << '-' << "1.0" << " " << "0.0" << '\n';
			myfile << "0.0" << " " << '-' << "1.0" << " " << "0.0" << '\n';
			myfile << "0.0" << " " << '-' << "1.0" << " " << "0.0" << '\n';
			myfile << "0.0" << " " << '-' << "1.0" << " " << "0.0" << '\n';
			//face frontal
			myfile << "0.0" << " " << "0.0" << " " << "1.0" << '\n';
			myfile << "0.0" << " " << "0.0" << " " << "1.0" << '\n';
			myfile << "0.0" << " " << "0.0" << " " << "1.0" << '\n';
			myfile << "0.0" << " " << "0.0" << " " << "1.0" << '\n';
			myfile << "0.0" << " " << "0.0" << " " << "1.0" << '\n';
			myfile << "0.0" << " " << "0.0" << " " << "1.0" << '\n';
			//face traseira
			myfile << "0.0" << " " << "0.0" << " " << '-' << "1.0" << '\n';
			myfile << "0.0" << " " << "0.0" << " " << '-' << "1.0" << '\n';
			myfile << "0.0" << " " << "0.0" << " " << '-' << "1.0" << '\n';
			myfile << "0.0" << " " << "0.0" << " " << '-' << "1.0" << '\n';
			myfile << "0.0" << " " << "0.0" << " " << '-' << "1.0" << '\n';
			myfile << "0.0" << " " << "0.0" << " " << '-' << "1.0" << '\n';
			//face lateral esquerda
			myfile << '-' << "1.0" << " " << "0.0" << " " << "0.0" << '\n';
			myfile << '-' << "1.0" << " " << "0.0" << " " << "0.0" << '\n';
			myfile << '-' << "1.0" << " " << "0.0" << " " << "0.0" << '\n';
			myfile << '-' << "1.0" << " " << "0.0" << " " << "0.0" << '\n';
			myfile << '-' << "1.0" << " " << "0.0" << " " << "0.0" << '\n';
			myfile << '-' << "1.0" << " " << "0.0" << " " << "0.0" << '\n';
			//face lateral direita
			myfile << "1.0" << " " << "0.0" << " " << "0.0" << '\n';
			myfile << "1.0" << " " << "0.0" << " " << "0.0" << '\n';
			myfile << "1.0" << " " << "0.0" << " " << "0.0" << '\n';
			myfile << "1.0" << " " << "0.0" << " " << "0.0" << '\n';
			myfile << "1.0" << " " << "0.0" << " " << "0.0" << '\n';
			myfile << "1.0" << " " << "0.0" << " " << "0.0" << '\n';
		}
	}
	myfile.close();
}

void textureBox(float xDimension, float yDimension, float zDimension, int divisions, std::string name) {
	ofstream myfile;
	myfile.open("../../SistemaSolar/" + name);

	float tex = 1.0f / divisions;

	for (int i = 0; i < divisions; i++) {
		for (int j = 0; j < divisions; j++) {

			float ni = (float)i;
			float nj = (float)j;

			//face superior
			myfile << 1.0f - nj / divisions << " " << ni / divisions << '\n';
			myfile << 1.0f - nj / divisions << " " << ni / divisions + tex << '\n';
			myfile << 1.0f - nj / divisions - tex << " " << ni / divisions + tex << '\n';

			myfile << 1.0f - nj / divisions - tex << " " << ni / divisions << '\n';
			myfile << 1.0f - nj / divisions << " " << ni / divisions << '\n';
			myfile << 1.0f - nj / divisions - tex << " " << ni / divisions + tex << '\n';

			//face inferior
			myfile << nj / divisions + tex << " " << ni / divisions << '\n';
			myfile << nj / divisions + tex << " " << ni / divisions + tex << '\n';
			myfile << nj / divisions << " " << ni / divisions + tex << '\n';

			myfile << nj / divisions << " " << ni / divisions << '\n';
			myfile << nj / divisions + tex << " " << ni / divisions << '\n';
			myfile << nj / divisions << " " << ni / divisions + tex << '\n';

			//face frontal
			myfile << nj / divisions << " " << ni / divisions << '\n';
			myfile << nj / divisions + tex << " " << ni / divisions << '\n';
			myfile << nj / divisions << " " << ni / divisions + tex << '\n';

			myfile << nj / divisions << " " << ni / divisions + tex << '\n';
			myfile << nj / divisions + tex << " " << ni / divisions << '\n';
			myfile << nj / divisions + tex << " " << ni / divisions + tex << '\n';

			//face traseira
			myfile << 1.0f - nj / divisions << " " << ni / divisions << '\n';
			myfile << 1.0f - nj / divisions << " " << ni / divisions + tex << '\n';
			myfile << 1.0f - nj / divisions + tex << " " << ni / divisions + tex << '\n';

			myfile << 1.0f - nj / divisions + tex << " " << ni / divisions << '\n';
			myfile << 1.0f - nj / divisions << " " << ni / divisions << '\n';
			myfile << 1.0f - nj / divisions + tex << " " << ni / divisions + tex << '\n';

			//face lateral esquerda
			myfile << ni / divisions + tex << " " << nj / divisions << '\n';
			myfile << ni / divisions + tex << " " << nj / divisions + tex << '\n';
			myfile << ni / divisions << " " << nj / divisions + tex << '\n';

			myfile << ni / divisions << " " << nj / divisions << '\n';
			myfile << ni / divisions + tex << " " << nj / divisions << '\n';
			myfile << ni / divisions << " " << nj / divisions + tex << '\n';

			//face lateral direita
			myfile << ni / divisions << " " << nj / divisions << '\n';
			myfile << ni / divisions << " " << nj / divisions + tex << '\n';
			myfile << ni / divisions + tex << " " << nj / divisions + tex << '\n';

			myfile << ni / divisions + tex << " " << nj / divisions << '\n';
			myfile << ni / divisions << " " << nj / divisions << '\n';
			myfile << ni / divisions + tex << " " << nj / divisions + tex << '\n';

		}
	}
	myfile.close();
}

void drawEsfera(float radius, float stacks, float slices, std::string name) {
	float sliceStep = (float)(2 * M_PI) / slices;
	float stackStep = (float)M_PI / stacks;
	float x, y, z;
	float x2, y2, z2;
	float x3, y3, z3;
	float x4, y4, z4;
	float sliceAngle, stackAngle;
	ofstream myfile;
	myfile.open("../../SistemaSolar/" + name);

	for (int i = 1; i < stacks + 1; i++) {
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

void normaisEsfera(float radius, float stacks, float slices, std::string name) {
	float sliceStep = (float)(2 * M_PI) / slices;
	float stackStep = (float)M_PI / stacks;
	float x, y, z;
	float x2, y2, z2;
	float x3, y3, z3;
	float x4, y4, z4;
	float sliceAngle, stackAngle;
	ofstream myfile;
	myfile.open("../../SistemaSolar/" + name);

	for (int i = 1; i < stacks + 1; i++) {
		stackAngle = M_PI / 2 - ((double)i * stackStep); // starting from -pi to pi

		for (int j = 0; j <= slices; ++j)
		{
			sliceAngle = j * sliceStep; // starting from 0 to 2pi

			//Ponto 1
			z = cosf(stackAngle) * cosf(sliceAngle);	// r * cos(u) * cos(v)
			x = cosf(stackAngle) * sinf(sliceAngle);	// r * cos(u) * sin(v)
			y = sinf(stackAngle); // r * sin(v)

			//Ponto 2
			z2 = cosf(stackAngle) * cosf(sliceAngle + sliceStep);	// r * cos(u) * cos(v)
			x2 = cosf(stackAngle) * sinf(sliceAngle + sliceStep);	// r * cos(u) * sin(v)
			y2 = y;

			//Ponto 3
			z3 = cosf(stackAngle + stackStep) * cosf(sliceAngle);	// r * cos(u) * cos(v)
			x3 = cosf(stackAngle + stackStep) * sinf(sliceAngle);	// r * cos(u) * sin(v)
			y3 = sinf(stackAngle + stackStep); // r * sin(v)

			//Ponto 4
			z4 = cosf(stackAngle + stackStep) * cosf(sliceAngle + sliceStep);	// r * cos(u) * cos(v)
			x4 = cosf(stackAngle + stackStep) * sinf(sliceAngle + sliceStep);	// r * cos(u) * sin(v)
			y4 = sinf(stackAngle + stackStep); // r * sin(v)

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

void textureEsfera(float radius, float stacks, float slices, std::string name) {
	ofstream myfile;
	myfile.open("../../SistemaSolar/" + name);

	float xSlice = 1.0f / slices;
	float yStack = 1.0f / stacks;

	for (int i = 1; i < stacks + 1; i++) {
		float stackStep = 1.0f - ((double)i * yStack);

		for (int j = 0; j <= slices; ++j) {
			float sliceStep = j * xSlice;
			myfile << sliceStep << " " << stackStep << '\n';
			myfile << sliceStep + xSlice << " " << stackStep << '\n';
			myfile << sliceStep << " " << stackStep + yStack << '\n';

			if (i > 0) {
				myfile << sliceStep << " " << stackStep + yStack << '\n';
				myfile << sliceStep + xSlice << " " << stackStep << '\n';
				myfile << sliceStep + xSlice << " " << stackStep + yStack << '\n';
			}
		}
	}
}

void drawCone(float radius, float height, float slices, float stacks, std::string name) {
	ofstream myfile;
	myfile.open("../../SistemaSolar/" + name);

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

void normaisCone(float radius, float height, float slices, float stacks, std::string name) {
	ofstream myfile;
	myfile.open("../../SistemaSolar/" + name);

	float angle = ((2 * M_PI) / slices);
	float stackDiv = height / stacks;
	float stackRaio = radius / stacks;
	float totAlt = 0, totRaio = 0;

	for (int i = 0; i < stacks; i++) {
		totRaio = radius - (stackRaio * i);
		totAlt = stackDiv * i;

		for (int j = 0; j < slices; j++) {
			float x = sin(angle * j);
			float z = cos(angle * j);
			float y = totAlt;

			float x1 = sin((j + 1) * angle);
			float z1 = cos((j + 1) * angle);

			if (totAlt == 0) {
				myfile << "0.0" << " " << y << " " << "0.0" << '\n';
				myfile << x1 << " " << y << " " << z1 << '\n';
				myfile << x << " " << y << " " << z << '\n';
			}

			myfile << x << " " << y << " " << z << '\n';
			myfile << x1 << " " << y << " " << z1 << '\n';
			myfile << sin((j + 1) * angle) << " " << (y + stackDiv) << " " << cos((j + 1) * angle) << '\n';

			if (i < stacks - 1) {
				myfile << x << " " << y << " " << z << '\n';
				myfile << sin((j + 1) * angle) << " " << (y + stackDiv) << " " << cos((j + 1) * angle) << '\n';
				myfile << sin(j * angle) << " " << (y + stackDiv) << " " << cos(j * angle) << '\n';
			}
		}
	}
	myfile.close();
}

void textureCone(float radius, float height, float slices, float stacks, std::string name) {
	ofstream myfile;
	myfile.open("../../SistemaSolar/" + name);

	float angle = ((2 * M_PI) / slices);
	float stackDiv = height / stacks;

	float yStack = 0.625 / stacks;
	float xSlice = 1.0 / slices;

	for (int i = 0; i < stacks; i++) {

		for (int j = 0; j < slices; j++) {

			if (i == 0) {
				myfile << "0.8125" << " " << "0.1875" << " " << '\n';
				myfile << (0.1875 * sin((j + 1) * angle)) + 0.8125 << " " << (0.1875 * cos((j + 1) * angle)) + 0.1875 << '\n';
				myfile << (0.1875 * sin(j * angle)) + 0.8125 << " " << (0.1875 * cos(j * angle)) + 0.1875 << '\n';
			}

			myfile << (float)j * xSlice << " " << 0.375 + (float)i * yStack << '\n';
			myfile << (float)(j + 1) * xSlice << " " << 0.375 + (float)i * yStack << '\n';
			myfile << (float)(j + 1) * xSlice << " " << 0.375 + (float)(i + 1) * yStack << '\n';

			if (i < stacks - 1) {
				myfile << (float)j * xSlice << " " << 0.375 + (float)i * yStack << '\n';
				myfile << (float)(j + 1) * xSlice << " " << 0.375 + (float)(i + 1) * yStack << '\n';
				myfile << (float)j * xSlice << " " << 0.375 + (float)(i + 1) * yStack << '\n';
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

void normaisArc(float outradius, float inradius, int slices, std::string name) {
	ofstream myfile;
	myfile.open("../../SistemaSolar/" + name);

	for (int i = 0; i <= slices; i++) {

		myfile << "0.0" << " " << "1.0" << " " << "0.0" << '\n';
		myfile << "0.0" << " " << "1.0" << " " << "0.0" << '\n';
		myfile << "0.0" << " " << "1.0" << " " << "0.0" << '\n';

		myfile << "0.0" << " " << "1.0" << " " << "0.0" << '\n';
		myfile << "0.0" << " " << "1.0" << " " << "0.0" << '\n';
		myfile << "0.0" << " " << "1.0" << " " << "0.0" << '\n';

		myfile << "0.0" << " " << '-' << "1.0" << " " << "0.0" << '\n';
		myfile << "0.0" << " " << '-' << "1.0" << " " << "0.0" << '\n';
		myfile << "0.0" << " " << '-' << "1.0" << " " << "0.0" << '\n';

		myfile << "0.0" << " " << '-' << "1.0" << " " << "0.0" << '\n';
		myfile << "0.0" << " " << '-' << "1.0" << " " << "0.0" << '\n';
		myfile << "0.0" << " " << '-' << "1.0" << " " << "0.0" << '\n';
	}

	myfile.close();

}

void textureArc(int slices, std::string name) {
	ofstream myfile;
	myfile.open("../../SistemaSolar/" + name);

	for (int i = 0; i <= slices; i++) {

		myfile << 0.0 << " " << 1.0 << '\n';
		myfile << 1.0 << " " << 0.0 << '\n';
		myfile << 1.0 << " " << 1.0 << '\n';

		myfile << 1.0 << " " << 0.0 << '\n';
		myfile << 0.0 << " " << 1.0 << '\n';
		myfile << 0.0 << " " << 0.0 << '\n';

		myfile << 0.0 << " " << 1.0 << '\n';
		myfile << 1.0 << " " << 1.0 << '\n';
		myfile << 1.0 << " " << 0.0 << '\n';

		myfile << 1.0 << " " << 0.0 << '\n';
		myfile << 0.0 << " " << 0.0 << '\n';
		myfile << 0.0 << " " << 1.0 << '\n';
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

void normaisBelt(float inradius, float outradius, int slices, int stacks, std::string name) {
	ofstream myfile;
	myfile.open("../../SistemaSolar/" + name);


	for (int i = 0; i < stacks; i++) {
		float stackAngle = i * ((2 * M_PI) / stacks);
		float stackAngle1 = (i + 1) * ((2 * M_PI) / stacks);
		for (int j = 0; j < slices; j++) {
			float sliceAngle = j * ((2 * M_PI) / slices);
			float sliceAngle1 = (j + 1) * ((2 * M_PI) / slices);

			myfile << cos(stackAngle) * cos(sliceAngle1) << " " << sin(stackAngle) * cos(sliceAngle1) << " " << sin(sliceAngle1) << '\n';
			myfile << cos(stackAngle) * cos(sliceAngle) << " " << sin(stackAngle) * cos(sliceAngle) << " " << sin(sliceAngle) << '\n';
			myfile << cos(stackAngle1) * cos(sliceAngle1) << " " << sin(stackAngle1) * cos(sliceAngle1) << " " << sin(sliceAngle1) << '\n';

			myfile << cos(stackAngle) * cos(sliceAngle) << " " << sin(stackAngle) * cos(sliceAngle) << " " << sin(sliceAngle) << '\n';
			myfile << cos(stackAngle1) * cos(sliceAngle) << " " << sin(stackAngle1) * cos(sliceAngle) << " " << sin(sliceAngle) << '\n';
			myfile << cos(stackAngle1) * cos(sliceAngle1) << " " << sin(stackAngle1) * cos(sliceAngle1) << " " << sin(sliceAngle1) << '\n';
		}
	}
	myfile.close();

}

void textureBelt_version1(float inradius, float outradius, int slices, int stacks, std::string name) {
	ofstream myfile;
	myfile.open("../../SistemaSolar/" + name);

	float xStack = 1.0 / stacks;
	float ySlice = 1.0 / slices;
	float altura = 1.0;

	for (int i = 0; i < stacks; i++) {
		for (int j = 0; j < slices; j++) {

			myfile << (float)i * xStack << " " << altura - ((float)(j + 1) * ySlice) << '\n';
			myfile << (float)i * xStack << " " << altura - ((float)j * ySlice) << '\n';
			myfile << (float)(i + 1) * xStack << " " << altura - ((float)(j + 1) * ySlice) << '\n';

			myfile << (float)i * xStack << " " << altura - ((float)j * ySlice) << '\n';
			myfile << (float)(i + 1) * xStack << " " << altura - ((float)j * ySlice) << '\n';
			myfile << (float)(i + 1) * xStack << " " << altura - ((float)(j + 1) * ySlice) << '\n';
		}
	}
	myfile.close();
}

void textureBelt_version2(float inradius, float outradius, int slices, int stacks, std::string name) {
	ofstream myfile;
	myfile.open("../../SistemaSolar/" + name);

	float xSlice = 1.0 / slices;

	for (int i = 0; i < stacks; i++) {
		for (int j = 0; j < slices; j++) {

			myfile << (float)(j+1)*xSlice << " " << 0.0 << '\n';
			myfile << (float)j * xSlice << " " << 0.0 << '\n';
			myfile << (float)(j + 1) * xSlice << " " << 1.0 << '\n';

			myfile << (float)j * xSlice << " " << 0.0 << '\n';
			myfile << (float)j * xSlice << " " << 1.0 << '\n';
			myfile << (float)(j + 1) * xSlice << " " << 1.0 << '\n';
		}
	}
	myfile.close();
}
void drawCylinder(float radius, float height, int slices, std::string name) {

	ofstream myfile;
	myfile.open("../../SistemaSolar/" + name);

	int i;
	float sliceStep = (float)(2 * M_PI) / slices;

	// top
	for (i = 0; i < slices; i++) {
		myfile << 0 << " " << height << " " << 0 << '\n';
		myfile << sin(i * sliceStep) * radius << " " << height << " " << cos(i * sliceStep) * radius << '\n';
		myfile << sin((i + 1) * sliceStep) * radius << " " << height << " " << cos((i + 1) * sliceStep) * radius << '\n';
	}

	// bottom
	for (i = 0; i < slices; i++) {
		myfile << 0 << " " << 0 << " " << 0 << '\n';
		myfile << sin((i + 1) * sliceStep) * radius << " " << 0 << " " << cos((i + 1) * sliceStep) * radius << '\n';
		myfile << sin(i * sliceStep) * radius << " " << 0 << " " << cos(i * sliceStep) * radius << '\n';
	}

	// body
	for (i = 0; i <= slices; i++) {

		myfile << sin((i + 1) * sliceStep) * radius << " " << height << " " << cos((i + 1) * sliceStep) * radius << '\n';
		myfile << sin(i * sliceStep) * radius << " " << height << " " << cos(i * sliceStep) * radius << '\n';
		myfile << sin(i * sliceStep) * radius << " " << 0 << " " << cos(i * sliceStep) * radius << '\n';

		myfile << sin((i + 1) * sliceStep) * radius << " " << 0 << " " << cos((i + 1) * sliceStep) * radius << '\n';
		myfile << sin((i + 1) * sliceStep) * radius << " " << height << " " << cos((i + 1) * sliceStep) * radius << '\n';
		myfile << sin(i * sliceStep) * radius << " " << 0 << " " << cos(i * sliceStep) * radius << '\n';
	}
	myfile.close();
}

void normaisCylinder(float radius, float height, int slices, std::string name) {
	ofstream myfile;
	myfile.open("../../SistemaSolar/" + name);

	int i;
	float sliceStep = (float)(2 * M_PI) / slices;

	// top
	for (i = 0; i < slices; i++) {
		myfile << "0.0" << " " << "1.0" << " " << "0.0" << '\n';
		myfile << "0.0" << " " << "1.0" << " " << "0.0" << '\n';
		myfile << "0.0" << " " << "1.0" << " " << "0.0" << '\n';
	}

	// bottom
	for (i = 0; i < slices; i++) {
		myfile << "0.0" << " " << '-' << "1.0" << " " << "0.0" << '\n';
		myfile << "0.0" << " " << '-' << "1.0" << " " << "0.0" << '\n';
		myfile << "0.0" << " " << '-' << "1.0" << " " << "0.0" << '\n';
	}

	// body
	for (i = 0; i <= slices; i++) {

		myfile << sin((i + 1) * sliceStep) << " " << "0.0" << " " << cos((i + 1) * sliceStep) << '\n';
		myfile << sin(i * sliceStep) << " " << "0.0" << " " << cos(i * sliceStep) << '\n';
		myfile << sin(i * sliceStep) << " " << "0.0" << " " << cos(i * sliceStep) << '\n';

		myfile << sin((i + 1) * sliceStep) << " " << "0.0" << " " << cos((i + 1) * sliceStep) << '\n';
		myfile << sin((i + 1) * sliceStep) << " " << "0.0" << " " << cos((i + 1) * sliceStep) << '\n';
		myfile << sin(i * sliceStep) << " " << "0.0" << " " << cos(i * sliceStep) << '\n';
	}
	myfile.close();
}

void textureCylinder(float radius, float height, int slices, std::string name) {
	ofstream myfile;
	myfile.open("../../SistemaSolar/" + name);

	int i;
	float xShift = 1.0f / slices;
	float sliceStep = (float)(2 * M_PI) / slices;

	//top
	for (i = 0; i < slices; i++) {
		myfile << "0.4375" << " " << "0.1875" << '\n';
		myfile << 0.4375 + (0.1875 * sin(i * sliceStep)) << " " << 0.1875 + (0.1875 * cos(i * sliceStep)) << '\n';
		myfile << 0.4375 + (0.1875 * sin((i + 1) * sliceStep)) << " " << 0.1875 + (0.1875 * cos((i + 1) * sliceStep)) << '\n';
	}

	//bottom
	for (i = 0; i < slices; i++) {
		myfile << "0.8125" << " " << "0.1875" << '\n';
		myfile << 0.8125 + (0.1875 * sin((i + 1) * sliceStep)) << " " << 0.1875 + (0.1875 * cos((i + 1) * sliceStep)) << '\n';
		myfile << 0.8125 + (0.1875 * sin(i * sliceStep)) << " " << 0.1875 + (0.1875 * cos(i * sliceStep)) << '\n';
	}

	//body
	for (i = 0; i <= slices; i++) {
		myfile << (i + 1) * xShift << " " << "1" << '\n';
		myfile << i * xShift << " " << "1" << '\n';
		myfile << i * xShift << " " << "0.375" << '\n';

		myfile << (i + 1) * xShift << " " << "0.375" << '\n';
		myfile << (i + 1) * xShift << " " << "1" << '\n';
		myfile << i * xShift << " " << "0.375" << '\n';
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

void getBezierPoint(float u, float v, float* px, float* py, float* pz, float* normalV, float* normais, float* texs) {

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
	cross(derivU, derivV, normais);
	normalize(normais);
	texs[0] = u;
	texs[1] = v;
}

int genBezier(std::string name, int tesselation) {
	std::ofstream myfile, myfilenormais, myfiletexs;
	myfile.open("../../SistemaSolar/" + name + ".3d");
	myfilenormais.open("../../SistemaSolar/" + name + ".normals");
	myfiletexs.open("../../SistemaSolar/" + name + ".coords");
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
			float normais0[3];
			float normais1[3];
			float normais2[3];
			float normais3[3];
			float texs0[2];
			float texs1[2];
			float texs2[2];
			float texs3[2];
			for (int v = 0; v < tesselation; v++) {
				getBezierPoint(u / (float)tesselation, v / (float)tesselation, arrX, arrY, arrZ, p0, normais0, texs0);
				getBezierPoint((u + 1) / (float)tesselation, v / (float)tesselation, arrX, arrY, arrZ, p1, normais1, texs1);
				getBezierPoint(u / (float)tesselation, (v + 1) / (float)tesselation, arrX, arrY, arrZ, p2, normais2, texs2);
				getBezierPoint((u + 1) / (float)tesselation, (v + 1) / (float)tesselation, arrX, arrY, arrZ, p3, normais3, texs3);

				myfile << p0[0] << " " << p0[1] << " " << p0[2] << '\n';
				myfile << p3[0] << " " << p3[1] << " " << p3[2] << '\n';
				myfile << p2[0] << " " << p2[1] << " " << p2[2] << '\n';

				myfile << p1[0] << " " << p1[1] << " " << p1[2] << '\n';
				myfile << p3[0] << " " << p3[1] << " " << p3[2] << '\n';
				myfile << p0[0] << " " << p0[1] << " " << p0[2] << '\n';

				myfilenormais << normais0[0] << " " << normais0[1] << " " << normais0[2] << '\n';
				myfilenormais << normais3[0] << " " << normais3[1] << " " << normais3[2] << '\n';
				myfilenormais << normais2[0] << " " << normais2[1] << " " << normais2[2] << '\n';

				myfilenormais << normais1[0] << " " << normais1[1] << " " << normais1[2] << '\n';
				myfilenormais << normais3[0] << " " << normais3[1] << " " << normais3[2] << '\n';
				myfilenormais << normais0[0] << " " << normais0[1] << " " << normais0[2] << '\n';

				myfiletexs << texs0[0] << " " << texs0[1] << '\n';
				myfiletexs << texs3[0] << " " << texs3[1] << '\n';
				myfiletexs << texs2[0] << " " << texs2[1] << '\n';

				myfiletexs << texs1[0] << " " << texs1[1] << '\n';
				myfiletexs << texs3[0] << " " << texs3[1] << '\n';
				myfiletexs << texs0[0] << " " << texs0[1] << '\n';
			}
		}
	}
	myfile.close();
	myfilenormais.close();
	myfiletexs.close();
	return 0;
}


/*
---------- COMANDOS PARA TERMINAL ----------
	-figura- <------valores------>
	<----->
	cilindro raio altura slices filename (cilindro 2 5 20 cilindro)
	<----->
	cinto raiointerno raioexterno slices stacks texturing_mode filename (cinto 1.5 2 50 50 2 cinto)
	<----->
	arco raioexterno raiointerno slices filename (arco 2 1.5 50 arco)
	<----->
	cone raio altura slices stacks filename (cone 2 3 20 20 cone)
	<----->
	esfera raio stacks slices filename (esfera 2 30 30 esfera)
	<----->
	caixa comprimento altura largura divisoes filename (caixa 2 2 2 5 box)
	<----->
	plano comprimento_lado filename (plano 5 plano)
----------<--------<---->-------->----------
*/

int main(int argc, char** argv) {

	std::string argument(argv[1]);

	if (argument == "cilindro") {
		std::string raio(argv[2]);
		std::string altura(argv[3]);
		std::string slices(argv[4]);
		std::string name(argv[5]);

		std::string points = name + ".3d";
		std::string norms = name + ".normals";
		std::string texs = name + ".coords";

		drawCylinder(std::stof(raio), std::stof(altura), std::stof(slices), points);
		normaisCylinder(std::stof(raio), std::stof(altura), std::stof(slices), norms);
		textureCylinder(std::stof(raio), std::stof(altura), std::stof(slices), texs);
	}
	if (argument == "cone") {
		std::string raio(argv[2]);
		std::string altura(argv[3]);
		std::string slices(argv[4]);
		std::string stacks(argv[5]);
		std::string name(argv[6]);

		std::string points = name + ".3d";
		std::string norms = name + ".normals";
		std::string texs = name + ".coords";

		drawCone(std::stof(raio), std::stof(altura), std::stof(slices), std::stof(stacks), points);
		normaisCone(std::stof(raio), std::stof(altura), std::stof(slices), std::stof(stacks), norms);
		textureCone(std::stof(raio), std::stof(altura), std::stof(slices), std::stof(stacks), texs);
	}

	if (argument == "arco") {
		std::string raioexterno(argv[2]);
		std::string raiointerno(argv[3]);
		std::string slices(argv[4]);
		std::string name(argv[5]);

		std::string points = name + ".3d";
		std::string norms = name + ".normals";
		std::string texs = name + ".coords";
		drawArc(std::stof(raioexterno), std::stof(raiointerno), std::stof(slices), points);
		normaisArc(std::stof(raioexterno), std::stof(raiointerno), std::stof(slices), norms);
		textureArc(std::stof(slices), texs);
	}
	if (argument == "cinto") {
		std::string raiointerno(argv[2]);
		std::string raioexterno(argv[3]);
		std::string slices(argv[4]);
		std::string stacks(argv[5]);
		int texturing_mode = std::stof(argv[6]);
		std::string name(argv[7]);

		std::string points = name + ".3d";
		std::string norms = name + ".normals";
		std::string texs = name + ".coords";

		drawBelt(std::stof(raiointerno), std::stof(raioexterno), std::stof(slices), std::stof(stacks), points);
		normaisBelt(std::stof(raiointerno), std::stof(raioexterno), std::stof(slices), std::stof(stacks), norms);

		if(texturing_mode == 1)
			textureBelt_version1(std::stof(raiointerno), std::stof(raioexterno), std::stof(slices), std::stof(stacks), texs);
		else
			textureBelt_version2(std::stof(raiointerno), std::stof(raioexterno), std::stof(slices), std::stof(stacks), texs);
	}
	if (argument == "esfera") {
		std::string raio(argv[2]);
		std::string stacks(argv[3]);
		std::string slices(argv[4]);
		std::string name(argv[5]);

		std::string points = name + ".3d";
		std::string norms = name + ".normals";
		std::string texs = name + ".coords";
		drawEsfera(std::stof(raio), std::stof(stacks), std::stof(slices), points);
		normaisEsfera(std::stof(raio), std::stof(stacks), std::stof(slices), norms);
		textureEsfera(std::stof(raio), std::stof(stacks), std::stof(slices), texs);
	}
	if (argument == "caixa") {
		std::string comprimento(argv[2]);
		std::string altura(argv[3]);
		std::string largura(argv[4]);
		std::string divs(argv[5]);
		std::string name(argv[6]);

		std::string points = name + ".3d";
		std::string norms = name + ".normals";
		std::string texs = name + ".coords";
		drawBox(std::stof(comprimento), std::stof(altura), std::stof(largura), std::stof(divs), points);
		normaisBox(std::stof(comprimento), std::stof(altura), std::stof(largura), std::stof(divs), norms);
		textureBox(std::stof(comprimento), std::stof(altura), std::stof(largura), std::stof(divs), texs);
	}
	if (argument == "plano") {
		std::string comprimento(argv[2]);
		std::string name(argv[3]);

		std::string points = name + ".3d";
		std::string norms = name + ".normals";
		std::string texs = name + ".coords";

		drawPlan(std::stof(comprimento), points);
		normaisPlan(std::stof(comprimento), norms);
		texturePlan(std::stof(comprimento), texs);
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