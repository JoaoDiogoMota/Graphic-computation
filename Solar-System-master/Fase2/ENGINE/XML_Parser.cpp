#include "XML_Parser.h"

#define line1 "../../SistemaSolar/"

xmlData dadosXML(xmlData insert, tinyxml2::XMLElement* group) {
    int ordem = 1;
    tinyxml2::XMLNode* transformacoes;
    transformacoes = group->FirstChild();
    
    while (transformacoes != nullptr && ordem <= 3) {
        const char* value = transformacoes->Value();
        std::string trans(value);
        tinyxml2::XMLElement* transformacao;

        if (trans == "translate") {
            transformacao = group->FirstChildElement("translate");
            if (transformacao) {
                const char* x = transformacao->Attribute("X");
                const char* y = transformacao->Attribute("Y");
                const char* z = transformacao->Attribute("Z");

                if (x != nullptr) { insert.setTranslateX(atof(x)); }
                if (y != nullptr) { insert.setTranslateY(atof(y)); }
                if (z != nullptr) { insert.setTranslateZ(atof(z)); }
                insert.setOrdemTr(ordem);
                ordem++;
            }
        }
        if (trans == "rotate") {
            transformacao = group->FirstChildElement("rotate");
            if (transformacao) {
                const char* x = transformacao->Attribute("axisX");
                const char* y = transformacao->Attribute("axisY");
                const char* z = transformacao->Attribute("axisZ");
                const char* ang = transformacao->Attribute("angle");

                if (x != nullptr) { insert.setRotateX(atof(x)); }
                if (y != nullptr) { insert.setRotateY(atof(y)); }
                if (z != nullptr) { insert.setRotateZ(atof(z)); }
                if (ang != nullptr) { insert.setRotateAngulo(atof(ang)); }
                insert.setOrdemRt(ordem);
                ordem++;
            }
        }
        if (trans == "scale") {
            transformacao = group->FirstChildElement("scale");
            if (transformacao) {
                const char* x = transformacao->Attribute("X");
                const char* y = transformacao->Attribute("Y");
                const char* z = transformacao->Attribute("Z");

                if (x != nullptr) { insert.setScaleX(atof(x)); }
                if (y != nullptr) { insert.setScaleY(atof(y)); }
                if (z != nullptr) { insert.setScaleZ(atof(z)); }
                insert.setOrdemSc(ordem);
                ordem++;
            }
        }    
        transformacoes = transformacoes->NextSibling();
    }

    tinyxml2::XMLElement* modelos;
    modelos = group->FirstChildElement("models");
    if (modelos) {
        tinyxml2::XMLElement* modelo;
        modelo = modelos->FirstChildElement("model");
        while (modelo) {
            const char* ficheiro = modelo->Attribute("file");
            if (ficheiro != nullptr) {insert.addFile(std::string(ficheiro));}
            modelo = modelo->NextSiblingElement("model");
        }
    }
    return insert;
}
std::vector<xmlData> getModelosFilhos(tinyxml2::XMLElement* pai) {
    
    tinyxml2::XMLElement* filho = pai->FirstChildElement("group");
    std::vector<xmlData> toReturn;
    
    while (filho) {
        xmlData insert(1);
        insert = dadosXML(insert, filho);
        std::vector<xmlData> filhos = getModelosFilhos(filho);
        insert.setFilhos(filhos);
        toReturn.push_back(insert);
        filho = filho->NextSiblingElement("group");
    }

    return toReturn;
}
std::vector<xmlData> initParsing(std::string ficheiro) {
    
    tinyxml2::XMLDocument doc;
    std::string file_location = line1 + ficheiro;
    std::vector<xmlData> toReturn;

    doc.LoadFile(file_location.c_str());

    if(doc.ErrorID() == 0){
        tinyxml2::XMLElement* scene;
        scene = doc.FirstChildElement("scene");
        
        if (scene) {
            tinyxml2::XMLElement* group;
            group = scene->FirstChildElement("group");
 
            while (group) {              
                xmlData insert(1);
                insert = dadosXML(insert, group);
                std::vector<xmlData> filhos = getModelosFilhos(group);
                insert.setFilhos(filhos);
                toReturn.push_back(insert);
                group = group->NextSiblingElement("group");
            }
        }
    }

    return toReturn;
}
