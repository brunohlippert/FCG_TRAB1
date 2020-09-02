//
//  Poligono.cpp
//  OpenGLTest
//
//  Created by Márcio Sarroglia Pinho on 18/08/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//

#include "Poligono.h"

Poligono::Poligono()
{

}

void Poligono::insereVertice(Ponto p)
{
    Vertices.push_back(p);
}

Ponto Poligono::getVertice(int i)
{
    return Vertices[i];
}
void Poligono::desenhaPoligono()
{
    glBegin(GL_LINE_LOOP);
    for (int i=0; i<Vertices.size(); i++)
        glVertex3f(Vertices[i].x,Vertices[i].y,Vertices[i].z);
    glEnd();
}
void Poligono::desenhaVertices()
{
    glBegin(GL_POINTS);
    for (int i=0; i<Vertices.size(); i++)
        glVertex3f(Vertices[i].x,Vertices[i].y,Vertices[i].z);
    glEnd();
}
void Poligono::imprime()
{
    for (int i=0; i<Vertices.size(); i++)
        Vertices[i].imprime();
}
unsigned long Poligono::getNVertices()
{
    return Vertices.size();
}

/**
 * Calculo o index do ponto com menor Y no poligono.
 *
 * @return int index do ponto.
 */
int Poligono::getMinimo(){
    Ponto auxMinimo = getVertice(0);
    int intAuxMinimo = 0;
    for (int i=1; i< getNVertices(); i++){
        if(getVertice(i).y < auxMinimo.y){
            auxMinimo = getVertice(i);
            intAuxMinimo = i;
        }
    }

    return intAuxMinimo;
}

/**
 * Calculo o index do ponto com maior Y no poligono.
 *
 * @return int index do ponto.
 */
int Poligono::getMaximo(){
    Ponto auxMax = getVertice(0);
    int intAuxMax = 0;
    for (int i=1; i< getNVertices(); i++){
        if(getVertice(i).y > auxMax.y){
            auxMax = getVertice(i);
            intAuxMax = i;
        }
    }

    return intAuxMax;
}

/**
 * Copia os vertices do Poligono para um vetor a parte.
 *
 * @return vector<Ponto> contendo os pontos do poligono.
 */
vector <Ponto> Poligono::cpyPontosMapa(){
    vector <Ponto> vertices;
    for (int i=0; i< getNVertices(); i++){
        Ponto vAtual = getVertice(i);
        vertices.push_back(Ponto(vAtual.x, vAtual.y));
    }

    return vertices;
}
