//
//  Ponto.cpp
//  OpenGLTest
//
//  Created by Márcio Sarroglia Pinho on 18/08/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//

#include "Ponto.h"
Ponto::Ponto ()
{
    x=y=z=0;
}
Ponto::Ponto(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}
void Ponto::set(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}
void Ponto::imprime() {
    cout << "(" << x << ", " << y << ")" << flush;
}


void Ponto::desenha()
{
    glColor3f(this->red, this->green, this->blue);
    glBegin(GL_POINTS);
    glVertex3f(this->x, this->y, this->z);
    glEnd();
}

void Ponto::setaCor(float red, float green, float blue){
    this->red = red;
    this->green = green;
    this->blue = blue;
}
