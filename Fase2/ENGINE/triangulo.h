#ifndef ENGINE_TRIANGULO_H
#define ENGINE_TRIANGULO_H

class triangulo{

  float trianguloX1;
  float trianguloY1;
  float trianguloZ1;

  float trianguloX2;
  float trianguloY2;
  float trianguloZ2;

  float trianguloX3;
  float trianguloY3;
  float trianguloZ3;

public:

  triangulo(float vX1,float vY1,float vZ1,float vX2,float vY2,float vZ2,float vX3,float vY3,float vZ3);

  float getVerticeX1();
  float getVerticeY1();
  float getVerticeZ1();

  float getVerticeX2();
  float getVerticeY2();
  float getVerticeZ2();

  float getVerticeX3();
  float getVerticeY3();
  float getVerticeZ3();

};

#endif //ENGINE_TRIANGULO_H