#ifndef DRAW_H
#define DRAW_H

#ifdef _WIN32
	#include <windows.h>
#endif

#include <GL/glut.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <vector>

#include "triangulo.h"

void startDrawing(int argc, char **argv, std::vector<std::vector<triangulo>> figuras);

#endif //DRAW_H
