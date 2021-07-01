#ifndef DRAW_H
#define DRAW_H

#ifdef _WIN32
	#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/glut.h>
#include <IL/il.h>


#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "triangulo.h"
#include "xmlData.h"
#include "light.h"

void startDrawing(int argc, char **argv, std::vector<xmlData> figuras,std::vector<light> luz);

#endif //DRAW_H
