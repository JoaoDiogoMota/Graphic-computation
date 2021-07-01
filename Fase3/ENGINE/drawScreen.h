#ifndef DRAW_H
#define DRAW_H

#ifdef _WIN32
	#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/glut.h>


#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <vector>

#include "triangulo.h"
#include "xmlData.h"

void startDrawing(int argc, char **argv, std::vector<xmlData> figuras);

#endif //DRAW_H
