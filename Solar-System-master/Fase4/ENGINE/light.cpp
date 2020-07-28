#include "light.h"

light::light(std::string name) {
	tipo = name;
	pointX = 0;
	pointY = 0;
	pointZ = 0;
	dirX = 0;
	dirY = 0;
	dirZ = 0;
	phi = 0;
	theta = 0;
}

std::string light::getTipo() { return tipo; }

float light::getPointX() { return pointX; }
float light::getPointY() { return pointY; }
float light::getPointZ() { return pointZ; }

float light::getDirX() { return dirX; }
float light::getDirY() { return dirY; }
float light::getDirZ() { return dirZ; }

float light::getPhi() { return phi; }
float light::getTheta() { return theta; }

void light::setTipo(std::string tp) { tipo = tp; }

void light::setPointX(float val) { pointX = val; }
void light::setPointY(float val) { pointY = val; }
void light::setPointZ(float val) { pointZ = val; }

void light::setDirX(float val) { dirX = val; }
void light::setDirY(float val) { dirY = val; }
void light::setDirZ(float val) { dirZ = val; }

void light::setTheta(float val) { theta = val; }
void light::setPhi(float val) { phi = val; }