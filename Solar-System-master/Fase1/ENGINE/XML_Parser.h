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

std::vector<std::string> startParsing(std::string ficheiro);

#endif //PARSE_H