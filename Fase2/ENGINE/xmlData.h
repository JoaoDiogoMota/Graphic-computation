#ifndef ENGINE_XMLDATA_H
#define ENGINE_XMLDATA_H

#include <vector>
#include <string>

#include "triangulo.h"

class xmlData {

    std::vector<std::string> modelos;
    std::vector<triangulo> pontos;

    float translateX;
    float translateY;
    float translateZ;

    int rotateX;
    int rotateY;
    int rotateZ;
    float rotateAngulo;

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
    void setFilhos(std::vector<xmlData> fl);
    void setTranslateX(float x);
    void setTranslateY(float y);
    void setTranslateZ(float z);
    void setRotateX(int x);
    void setRotateY(int y);
    void setRotateZ(int z);
    void setRotateAngulo(float ang);
    void setScaleX(float x);
    void setScaleY(float y);
    void setScaleZ(float z);
    void setOrdemTr(int ord);
    void setOrdemRt(int ord);
    void setOrdemSc(int ord);
    void addFile(std::string ficheiro);

    std::vector<triangulo> getPontos();
    std::vector<std::string> getFileName();
    std::vector<xmlData> getFilhos();
    float getTranslateX();
    float getTranslateY();
    float getTranslateZ();
    int getRotateX();
    int getRotateY();
    int getRotateZ();
    float getRotateAngulo();
    float getScaleX();
    float getScaleY();
    float getScaleZ();
    int getOrdemTr();
    int getOrdemRt();
    int getOrdemSc();

    int getNumTransformacoes();
};
#endif //ENGINE_XMLDATA_H
