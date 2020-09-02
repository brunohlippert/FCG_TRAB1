//
//  Aresta.cpp
//  OpenGLTest

#include "Aresta.h"

Aresta::Aresta(Ponto p1, Ponto p2){
    //Mantem sempre o menor y no P1, para depois
    //Podermos calcular o intervalo de faixas de
    //Forma correta
    if(p1.y <= p2.y){
        ponto1 = p1;
        ponto2 = p2;
    } else {
        ponto1 = p2;
        ponto2 = p1;
    }

}

Ponto Aresta::getP1(){
    return ponto1;
}
Ponto Aresta::getP2(){
    return ponto2;
}


