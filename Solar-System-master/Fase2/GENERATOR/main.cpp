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

/*
---------- COMANDOS PARA TERMINAL ----------

	-figura- <------valores------>

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

int main(int argc, char **argv) {

	std::string argument(argv[1]);

	if (argument == "cone") {
		std::string raio(argv[2]);
		std::string altura(argv[3]);
		std::string slices(argv[4]);
		std::string stacks(argv[5]);
		std::string name(argv[6]);

		drawCone(std::stof(raio),std::stof(altura),std::stof(slices),std::stof(stacks),name);
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
	return 1;
}
