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

std::vector<std::vector<triangulo>> getVertices(std::vector<std::string> ficheiros);

#endif //LOADER_H