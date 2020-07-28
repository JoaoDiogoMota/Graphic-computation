#include "XML_Parser.h"

#define line1 "../../SistemaSolar/"

std::vector<std::string> startParsing(std::string ficheiro){

    tinyxml2::XMLDocument doc;
    std::vector<std::string> ret;

    std::string f_location = line1+ficheiro;
    doc.LoadFile(f_location.c_str());
    if (doc.ErrorID() == 0)
	  {
      std::cout << "Success" << std::endl;
      tinyxml2::XMLElement *scene;
      scene = doc.FirstChildElement("scene");
      if(scene){

        tinyxml2::XMLElement* modelos;
        modelos = scene->FirstChildElement("model");

        while (modelos)
        {
          std::string ficheiro = modelos->FirstAttribute()->Value();
          std::cout << ficheiro << std::endl;

          ret.push_back(ficheiro);

          modelos = modelos->NextSiblingElement("model");
	      }
      }
    }
    else
    {
      std::cout << "Failure" << std::endl;
    }

    return ret;
}
