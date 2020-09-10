//
//  Ponto.h
//  OpenGLTest
//
//  Created by Márcio Sarroglia Pinho on 18/08/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//

#ifndef Ponto_hpp
#define Ponto_hpp

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

class Ponto {

public:
    float x,y,z;
    float red,green,blue;
    Ponto ();
    Ponto(float x, float y, float z=0);
    void set(float x, float y, float z=0);
    void imprime();
    void desenha();
    void setaCor(float red, float green, float blue);
} ;


#endif /* Ponto_hpp */
