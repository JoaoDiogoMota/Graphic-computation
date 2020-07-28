#include "triangulo.h"

triangulo::triangulo(float vX1,float vY1,float vZ1,float vX2,float vY2,float vZ2,float vX3,float vY3,float vZ3){

  trianguloX1 = vX1;
  trianguloY1 = vY1;
  trianguloZ1 = vZ1;

  trianguloX2 = vX2;
  trianguloY2 = vY2;
  trianguloZ2 = vZ2;

  trianguloX3 = vX3;
  trianguloY3 = vY3;
  trianguloZ3 = vZ3;

}

float triangulo::getVerticeX1(){return trianguloX1;}
float triangulo::getVerticeY1(){return trianguloY1;}
float triangulo::getVerticeZ1(){return trianguloZ1;}

float triangulo::getVerticeX2(){return trianguloX2;}
float triangulo::getVerticeY2(){return trianguloY2;}
float triangulo::getVerticeZ2(){return trianguloZ2;}

float triangulo::getVerticeX3(){return trianguloX3;}
float triangulo::getVerticeY3(){return trianguloY3;}
float triangulo::getVerticeZ3(){return trianguloZ3;}
