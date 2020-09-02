//
//  Faixa.cpp
//  OpenGLTest

#include "Faixa.h"

Faixa::Faixa(float y, float alt){
    yInicial = y;
    altura = alt;
}

void Faixa::desenhaFaixa(float xMin, float xMax){
    glBegin(GL_LINES);
        glVertex2f(xMin,yInicial);
        glVertex2f(xMax,yInicial);
    glEnd();
}

void Faixa::addAresta(Aresta novaAresta){
    arestas.push_back(novaAresta);
}

vector <Aresta> Faixa::getArestas(){
    return arestas;
}


