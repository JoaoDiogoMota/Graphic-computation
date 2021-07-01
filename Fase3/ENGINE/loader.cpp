
#include "loader.h"

#define line1 "../../SistemaSolar/"

std::vector<std::vector<triangulo>> getVertices(std::vector<std::string> ficheiros){

  std::vector<std::vector<triangulo>> ret;
  FILE *fp;
  float x1,y1,z1,x2,y2,z2,x3,y3,z3;

  for(unsigned int i = 0; i < ficheiros.size() ;i++){
    std::string ficheiro = ficheiros.at(i);
    std::string localizacao = line1 + ficheiro;
    std::vector<triangulo> triangulos;
    int read;
    fp = fopen(localizacao.c_str(),"r");

    if(!fp){
      printf("can't open file %s\n", ficheiro.c_str());
      _exit(1);
    }
    else{
      while(!(feof(fp))){
        read = fscanf(fp,"%f %f %f %f %f %f %f %f %f",&x1,&y1,&z1,&x2,&y2,&z2,&x3,&y3,&z3);

        if(read == 9){
          triangulo tr = triangulo(x1,y1,z1,x2,y2,z2,x3,y3,z3);
          triangulos.push_back(tr);
        }
      }
    }
    ret.push_back(triangulos);
  }

  return ret;
}

xmlData loadFicheiros(xmlData nodo) {
    std::vector<triangulo> pontos;
    FILE* fp;
    float x1, y1, z1, x2, y2, z2, x3, y3, z3;

    std::vector<std::string> ficheiros = nodo.getFileName();
    for (unsigned int i = 0; i < ficheiros.size(); i++) {
        std::string ficheiro = ficheiros.at(i);
        std::string localizacao = line1 + ficheiro;
        int read;
        fp = fopen(localizacao.c_str(), "r");

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
        }
    }
    nodo.setPontos(pontos);
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
