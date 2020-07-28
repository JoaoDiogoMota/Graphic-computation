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
                const char* tipo = transformacao->Attribute("time");
                if (tipo != nullptr) {
                    float time = atof(tipo);
                    insert.setTranslateTime(time);
                    
                    tinyxml2::XMLElement* controlPoints;
                    controlPoints = transformacao->FirstChildElement("point");
                    while (controlPoints) {
                        const char* x = controlPoints->Attribute("X");
                        const char* y = controlPoints->Attribute("Y");
                        const char* z = controlPoints->Attribute("Z");
                        if (x!=nullptr && y!=nullptr && z!=nullptr) { insert.addPontosControlo(atof(x),atof(y),atof(z)); }
                        else{std::cout << "Ficheiro mal formatado! _exiting program." << std::endl;exit(EXIT_FAILURE);}
                        controlPoints = controlPoints->NextSiblingElement("point");
                    }
                }
                else {
                    const char* x = transformacao->Attribute("X");
                    const char* y = transformacao->Attribute("Y");
                    const char* z = transformacao->Attribute("Z");

                    if (x != nullptr) { insert.setTranslateX(atof(x)); }
                    if (y != nullptr) { insert.setTranslateY(atof(y)); }
                    if (z != nullptr) { insert.setTranslateZ(atof(z)); }
                }
                insert.setOrdemTr(ordem);
                ordem++;
            }
        }
        if (trans == "rotate") {
            transformacao = group->FirstChildElement("rotate");
            if (transformacao) {
                const char* tipo = transformacao->Attribute("time");
                const char* ang = transformacao->Attribute("angle");
                if (tipo != nullptr) {
                    insert.setRotateTime(atof(tipo));
                }
                if (ang != nullptr){ 
                    insert.setRotateAngulo(atof(ang)); 
                }

                const char* x = transformacao->Attribute("axisX");
                const char* y = transformacao->Attribute("axisY");
                const char* z = transformacao->Attribute("axisZ");
                if (x != nullptr){insert.setRotateX(atof(x));}
                if (y != nullptr){insert.setRotateY(atof(y));}
                if (z != nullptr){insert.setRotateZ(atof(z));}
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

            const char* tex = modelo->Attribute("texture");
            const char* dr = modelo->Attribute("diffR");
            const char* dg = modelo->Attribute("diffG");
            const char* db = modelo->Attribute("diffB");
            const char* sr = modelo->Attribute("specR");
            const char* sg = modelo->Attribute("specG");
            const char* sb = modelo->Attribute("specB");
            const char* sh = modelo->Attribute("shine");
            const char* er = modelo->Attribute("emR");
            const char* eg = modelo->Attribute("emG");
            const char* eb = modelo->Attribute("emB");
            const char* ar = modelo->Attribute("ambR");
            const char* ag = modelo->Attribute("ambG");
            const char* ab = modelo->Attribute("ambB");

            obj_Light mats(0);

            if (tex != nullptr)
                mats.setTexture(std::string(tex));
            if (dr != nullptr) {
                float ins = atof(dr);
                mats.setDR(ins);
                mats.setDV(1);
            }
            if (dg != nullptr) {
                float ins = atof(dg);
                mats.setDG(ins);
                mats.setDV(1);
            }
            if (db != nullptr) {
                float ins = atof(db);
                mats.setDB(ins);
                mats.setDV(1);
            }
            if (sr != nullptr) {
                float ins = atof(sr);
                mats.setSR(ins);
                mats.setSV(1);
            }
            if (sg != nullptr) {
                float ins = atof(sg);
                mats.setSG(ins);
                mats.setSV(1);
            }
            if (sb != nullptr) {
                float ins = atof(sb);
                mats.setSB(ins);
                mats.setSV(1);
            }
            if (sh != nullptr) {
                float ins = atof(sh);
                mats.setShine(ins);
                mats.setSV(1);
            }
            if (er != nullptr) {
                float ins = atof(er);
                mats.setER(ins);
                mats.setEV(1);
            }
            if (eg != nullptr) {
                float ins = atof(eg);
                mats.setEG(ins);
                mats.setEV(1);
            }
            if (eb != nullptr) {
                float ins = atof(eb);
                mats.setEB(ins);
                mats.setEV(1);
            }
            if (ar != nullptr) {
                float ins = atof(ar);
                mats.setAR(ins);
                mats.setAV(1);
            }
            if (ag != nullptr) {
                float ins = atof(ag);
                mats.setAG(ins);
                mats.setAV(1);
            }
            if (ab != nullptr) {
                float ins = atof(ab);
                mats.setAB(ins);
                mats.setAV(1);
            }

            insert.addMateriais(mats);

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

std::vector<light> getXMLight(std::string file_name) {
    tinyxml2::XMLDocument doc;
    std::string file_location = line1 + file_name;
    std::vector<light> listToReturn;

    doc.LoadFile(file_location.c_str());

    if (doc.ErrorID() == 0) {
        tinyxml2::XMLElement* scene;
        scene = doc.FirstChildElement("scene");

        if (scene) {
            tinyxml2::XMLElement* lt;
            lt = scene->FirstChildElement("lights");
           
            if (lt) {
                std::string ls(lt->FirstChild()->Value());
                light toReturn("empty");
          
                if (ls == "light") {
                    tinyxml2::XMLElement* l = lt->FirstChildElement("light");
                    while (l) {
                        std::string tipo(l->Attribute("type"));
                        toReturn.setTipo(tipo);
                        const char* px = l->Attribute("posX");
                        const char* py = l->Attribute("posY");
                        const char* pz = l->Attribute("posZ");
                        const char* dx = l->Attribute("dirX");
                        const char* dy = l->Attribute("dirY");
                        const char* dz = l->Attribute("dirZ");
                        const char* phi = l->Attribute("phi");
                        const char* theta = l->Attribute("theta");

                        if (px != nullptr)
                            toReturn.setPointX(atof(px));
                        if (py != nullptr)
                            toReturn.setPointY(atof(py));
                        if (pz != nullptr)
                            toReturn.setPointZ(atof(pz));
                        if (dx != nullptr)
                            toReturn.setDirX(atof(dx));
                        if (dy != nullptr)
                            toReturn.setDirY(atof(dy));
                        if (dz != nullptr)
                            toReturn.setDirZ(atof(dz));
                        if (phi != nullptr)
                            toReturn.setPhi(atof(phi));
                        if (theta != nullptr)
                            toReturn.setTheta(atof(theta));
                        
                        listToReturn.push_back(toReturn);
                        l = l->NextSiblingElement("light");
                    }
                }
            }
        }
    }
    return listToReturn;
}