
//
//  Faixa.hpp
//  OpenGLTest

#ifndef Faixa_hpp
#define Faixa_hpp

#include <iostream>
using namespace std;


#ifdef WIN32
#include <windows.h>
#include <glut.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <glut.h>
#endif

#include "Aresta.h"
#include <vector>

class Faixa{
    float yInicial;
    float altura;
    vector <Aresta> arestas;
public:
    Faixa(float y, float alt);
    void desenhaFaixa(float xMin, float xMax);
    void addAresta(Aresta novaAresta);
    vector <Aresta> getArestas();
};

#endif
