#include "obj_Light.h"

obj_Light::obj_Light(float notEmptyBuilder)
{
    dValue = 0;
    diffR = 1.0;
    diffG = 1.0;
    diffB = 1.0;

    sValue = 0;
    specR = 1.0;
    specG = 1.0;
    specB = 1.0;

    eValue = 0;
    emR = notEmptyBuilder;
    emG = 0;
    emB = 0;

    aValue = 0;
    ambR = 0;
    ambG = 0;
    ambB = 0;

    shine = 32.0;

    texture = "none";
}

std::string obj_Light::getTexture(){return texture;}

int obj_Light::getDV(){return dValue;}
float obj_Light::getDR(){return diffR;}
float obj_Light::getDG(){return diffG;}
float obj_Light::getDB(){return diffB;}

int obj_Light::getSV(){return sValue;}
float obj_Light::getSR(){return specR;}
float obj_Light::getSG(){return specG;}
float obj_Light::getSB(){return specB;}

int obj_Light::getEV(){return eValue;}
float obj_Light::getER(){return emR;}
float obj_Light::getEG(){return emG;}
float obj_Light::getEB(){return emB;}

int obj_Light::getAV(){return aValue;}
float obj_Light::getAR(){return ambR;}
float obj_Light::getAG(){return ambG;}
float obj_Light::getAB(){return ambB;}

float obj_Light::getShine(){return shine;}

void obj_Light::setTexture(std::string ins){texture = ins;}

void obj_Light::setDV(int ins){dValue = ins;}
void obj_Light::setDR(float ins){diffR = ins;}
void obj_Light::setDG(float ins){diffG = ins;}
void obj_Light::setDB(float ins){diffB = ins;}

void obj_Light::setSV(int ins){sValue = ins;}
void obj_Light::setSR(float ins){specR = ins;}
void obj_Light::setSG(float ins){specG = ins;}
void obj_Light::setSB(float ins){specB = ins;}

void obj_Light::setEV(int ins){eValue = ins;}
void obj_Light::setER(float ins){emR = ins;}
void obj_Light::setEG(float ins){emG = ins;}
void obj_Light::setEB(float ins){emB = ins;}

void obj_Light::setAV(int ins){aValue = ins;}
void obj_Light::setAR(float ins){ambR = ins;}
void obj_Light::setAG(float ins){ambG = ins;}
void obj_Light::setAB(float ins){ambB = ins;}

void obj_Light::setShine(float ins){shine = ins;}
