#ifndef ENGINE_XMLDATA_H
#define ENGINE_XMLDATA_H

#include <vector>
#include <string>

#include "triangulo.h"
#include "obj_Light.h"

class xmlData {

    std::vector<std::string> modelos;
    std::vector<std::vector<triangulo>> pontos;
    std::vector<std::vector<triangulo>> coords;
    std::vector<std::vector<triangulo>> normais;
    std::vector<float> pontosDeControlo;
    std::vector<obj_Light> materiais;

    float translateX;
    float translateY;
    float translateZ;
    float timeTranslate;

    int rotateX;
    int rotateY;
    int rotateZ;
    float rotateAngulo;
    float timeRotate;

    float scaleX;
    float scaleY;
    float scaleZ;

    int ordemTranslate;
    int ordemRotate;
    int ordemScale;

    std::vector<xmlData> filhos;

public:

    xmlData(int ignore);
    
    void setPontos(std::vector<triangulo> pt);
    void setCoords(std::vector<triangulo> pt);
    void setNormais(std::vector<triangulo> pt);
    void setFilhos(std::vector<xmlData> fl);
    void addPontosControlo(float x, float y, float z);
    
    void setTranslateX(float x);
    void setTranslateY(float y);
    void setTranslateZ(float z);
    void setTranslateTime(float tm);

    void setRotateX(int x);
    void setRotateY(int y);
    void setRotateZ(int z);
    void setRotateAngulo(float ang);
    void setRotateTime(float time);

    void setScaleX(float x);
    void setScaleY(float y);
    void setScaleZ(float z);
    void setOrdemTr(int ord);
    void setOrdemRt(int ord);
    void setOrdemSc(int ord);
    void addFile(std::string ficheiro);
    void addMateriais(obj_Light ol);

    std::vector<std::vector<triangulo>> getPontos();
    std::vector<std::vector<triangulo>> getNormais();
    std::vector<std::vector<triangulo>> getCoords();
    std::vector<std::string> getFileName();
    std::vector<xmlData> getFilhos();
    std::vector<obj_Light> getMateriais();
    std::vector<float> getControlPoints();

    float getTranslateX();
    float getTranslateY();
    float getTranslateZ();
    float getTempoTr();

    int getRotateX();
    int getRotateY();
    int getRotateZ();
    float getRotateAngulo();
    float getTempoRt();

    float getScaleX();
    float getScaleY();
    float getScaleZ();

    int getOrdemTr();
    int getOrdemRt();
    int getOrdemSc();

    int getNumTransformacoes();
    int getNumControlPoints();
};
#endif //ENGINE_XMLDATA_H
