
#include "loader.h"

#define line1 "../../SistemaSolar/"

xmlData loadFicheiros(xmlData nodo) {
    std::vector<triangulo> pontos;
    std::vector<triangulo> normais;
    std::vector<triangulo> textura;
    FILE* fp;
    FILE* fp2;
    FILE* fp3;
    float x1, y1, z1, x2, y2, z2, x3, y3, z3;

    std::vector<std::string> ficheiros = nodo.getFileName();
    for (unsigned int i = 0; i < ficheiros.size(); i++) {
        std::string ficheiro = ficheiros.at(i);
        std::string fich3d = line1 + ficheiro + ".3d";
        std::string fichNm = line1 + ficheiro + ".normals";
        std::string fichCd = line1 + ficheiro + ".coords";
        
        int read;
        fp = fopen(fich3d.c_str(), "r");

        if (!fp) {
            printf("can't open file %s\n", ficheiro.c_str());
            _exit(1);
        }
        else {
            while (!(feof(fp))) {
                read = fscanf(fp, "%f %f %f %f %f %f %f %f %f", &x1, &y1, &z1, &x2, &y2, &z2, &x3, &y3, &z3);

                if (read == 9) {
                    triangulo tr = triangulo(x1, y1, z1, x2, y2, z2, x3, y3, z3);
                    pontos.push_back(tr);
                }
            }
        }fclose(fp);
           
        fp2 = fopen(fichNm.c_str(), "r");
        if (fp2) {
            while (!(feof(fp2))) {
                read = fscanf(fp2, "%f %f %f %f %f %f %f %f %f", &x1, &y1, &z1, &x2, &y2, &z2, &x3, &y3, &z3);

                if (read == 9) {
                    triangulo tr = triangulo(x1, y1, z1, x2, y2, z2, x3, y3, z3);
                    normais.push_back(tr);
                }
            }fclose(fp2);
        }

        fp3 = fopen(fichCd.c_str(), "r");
        if (fp3) {
            while (!(feof(fp3))) {
                read = fscanf(fp3, "%f %f %f %f %f %f", &x1, &y1, &x2, &y2, &x3, &y3);

                if (read == 6) {
                    triangulo tr = triangulo(x1, y1, -1, x2, y2, -1, x3, y3, -1);
                    textura.push_back(tr);
                }
            }fclose(fp3);
        }

        nodo.setPontos(pontos);
        nodo.setNormais(normais);
        nodo.setCoords(textura);
        pontos.clear();
        normais.clear();
        textura.clear();
    }  
    return nodo;
}
xmlData loadFilhos(xmlData pai) {
    std::vector<xmlData> filhos = pai.getFilhos();
    std::vector<xmlData> newFilhos;
    for (unsigned int i = 0; i < filhos.size(); i++) {
        xmlData infoFilho = filhos.at(i);
        infoFilho = loadFicheiros(infoFilho);
        infoFilho = loadFilhos(infoFilho);
        newFilhos.push_back(infoFilho);
    }
    pai.setFilhos(newFilhos);
    return pai;
}
std::vector<xmlData> initLoad(std::vector<xmlData> ficheiros) {
    std::vector<xmlData> newFicheiros;
    for (unsigned int i = 0; i < ficheiros.size(); i++) {
        xmlData info = ficheiros.at(i);
        info = loadFicheiros(info);
        info = loadFilhos(info);
        newFicheiros.push_back(info);
    }
    return newFicheiros;
}
