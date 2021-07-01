#ifndef LOADER_H
#define LOADER_H

#ifdef _WIN32
  #include <windows.h>
#endif

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "triangulo.h"
#include "xmlData.h"

std::vector<xmlData> initLoad(std::vector<xmlData> ficheiros);

#endif //LOADER_H