

//
//  Aresta.hpp
//  OpenGLTest

#ifndef Aresta_hpp
#define Aresta_hpp

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

#include "Ponto.h"

class Aresta{
    Ponto ponto1;
    Ponto ponto2;
public:
    Aresta(Ponto p1, Ponto p2);
    Ponto getP1();
    Ponto getP2();
};

#endif
