#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <list>
#include <map>
#include <set>
#include <vector>
#include <random>
#include "objeto.h"



Objeto::Objeto(int ide, int anc, int alt){
    this->id = ide;
    this->ancho = anc;
    this->alto = alt;
    this->rotacion = false;
}
        //Sobrecarga de constructor para coordenadas
Objeto::Objeto(int ide, int anc, int alt, int xx, int yy){
    this->id = ide;
    this->ancho = anc;
    this->alto = alt;
    this->rotacion = false;
    this->x = xx;
    this->y = yy;
}

int Objeto::right() const{
    return x + ancho - 1;
}

int Objeto::bottom() const{
    return y + alto - 1;
}

bool Objeto::getRotacion(){
    return this->rotacion;
}

int Objeto::getAncho(){
    return this->ancho;
}

int Objeto::getAlto(){
    return this->alto;
}

int Objeto::getId(){
    return this->id;
}

void Objeto::setAncho(int anc){
    this->ancho = anc;
}

void Objeto::setAlto(int alt){
    this->alto = alt;
}

void Objeto::rotar(){
    int aux = getAlto();
    setAlto(getAncho());
    setAncho(aux);
    if(getRotacion()){
        this->rotacion = false;
    }
    else{
        this->rotacion = true;
    }
}
