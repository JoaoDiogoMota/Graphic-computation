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
