#include "drawScreen.h"
#include "XML_Parser.h"
#include "loader.h"
#include "triangulo.h"

int main(int argc, char **argv) {
    std::vector<std::string> ficheiros = startParsing("XML_Teste.xml");
    std::vector<std::vector<triangulo>> figuras = getVertices(ficheiros);

    startDrawing(argc,argv,figuras);
}
