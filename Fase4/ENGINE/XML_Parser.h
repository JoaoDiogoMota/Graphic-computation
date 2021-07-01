#ifndef PARSE_H
#define PARSE_H

#ifdef _WIN32
    #include <windows.h>
#endif

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "tinyxml2.h"
#include "xmlData.h"
#include "triangulo.h"
#include "light.h"

std::vector<xmlData> initParsing(std::string ficheiro);
std::vector<light> getXMLight(std::string file_name);

#endif //PARSE_H