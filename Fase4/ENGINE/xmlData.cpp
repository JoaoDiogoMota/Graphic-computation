#include "xmlData.h"

xmlData::xmlData(int ignore) {
	
	translateX = 0;
	translateY = 0;
	translateZ = 0;
	timeTranslate = 0;
	
	scaleX = 1;
	scaleY = 1;
	scaleZ = 1;

	rotateX = 0;
	rotateY = 0;
	rotateZ = 0;
	rotateAngulo = 0;
	timeRotate = 0;

	ordemTranslate = 0;
	ordemRotate = 0;
	ordemScale = 0;

	timeRotate = 0;
	timeTranslate = 0;
}

void xmlData::setPontos(std::vector<triangulo> pt){pontos.push_back(pt);}
void xmlData::setCoords(std::vector<triangulo> pt){coords.push_back(pt);}
void xmlData::setNormais(std::vector<triangulo> pt){normais.push_back(pt);}
void xmlData::setFilhos(std::vector<xmlData> fl){filhos = fl;}
void xmlData::addPontosControlo(float x, float y, float z) {pontosDeControlo.push_back(x);pontosDeControlo.push_back(y);pontosDeControlo.push_back(z);}

void xmlData::setTranslateX(float x){translateX = x;}
void xmlData::setTranslateY(float y){translateY = y;}
void xmlData::setTranslateZ(float z){translateZ = z;}
void xmlData::setTranslateTime(float tm){timeTranslate = tm;}

void xmlData::setRotateX(int x){rotateX = x;}
void xmlData::setRotateY(int y){rotateY = y;}
void xmlData::setRotateZ(int z){rotateZ = z;}
void xmlData::setRotateAngulo(float ang){rotateAngulo = ang;}
void xmlData::setRotateTime(float time){timeRotate = time;}

void xmlData::setScaleX(float x){scaleX = x;}
void xmlData::setScaleY(float y){scaleY = y;}
void xmlData::setScaleZ(float z){scaleZ = z;}
void xmlData::setOrdemTr(int ord){ordemTranslate = ord;}
void xmlData::setOrdemRt(int ord){ordemRotate = ord;}
void xmlData::setOrdemSc(int ord){ordemScale = ord;}
void xmlData::addFile(std::string ficheiro){modelos.push_back(ficheiro);}
void xmlData::addMateriais(obj_Light ol) { materiais.push_back(ol); }

std::vector<std::vector<triangulo>> xmlData::getPontos() { return pontos; }
std::vector<std::vector<triangulo>> xmlData::getNormais() { return normais; }
std::vector<std::vector<triangulo>> xmlData::getCoords() { return coords; }
std::vector<std::string> xmlData::getFileName(){return modelos;}
std::vector<xmlData> xmlData::getFilhos() { return filhos; }
std::vector<obj_Light> xmlData::getMateriais() { return materiais; }
std::vector<float> xmlData::getControlPoints() { return pontosDeControlo; }

float xmlData::getTranslateX() { return translateX; }
float xmlData::getTranslateY() { return translateY; }
float xmlData::getTranslateZ() { return translateZ; }
float xmlData::getTempoTr() { return timeTranslate; }

int xmlData::getRotateX(){return rotateX;}
int xmlData::getRotateY(){return rotateY;}
int xmlData::getRotateZ(){return rotateZ;}
float xmlData::getRotateAngulo(){return rotateAngulo;}
float xmlData::getTempoRt(){return timeRotate;}

float xmlData::getScaleX(){return scaleX;}
float xmlData::getScaleY(){return scaleY;}
float xmlData::getScaleZ(){return scaleZ;}

int xmlData::getOrdemTr(){return ordemTranslate;}
int xmlData::getOrdemRt(){return ordemRotate;}
int xmlData::getOrdemSc(){return ordemScale;}

int xmlData::getNumTransformacoes() {
	int ret = 0;
	
	if (ordemTranslate > 0)
		ret++;
	if (ordemRotate > 0)
		ret++;
	if (ordemScale > 0)
		ret++;

	return ret;
}

int xmlData::getNumControlPoints() { return pontosDeControlo.size(); }