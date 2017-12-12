#ifndef OBJETO_H
#define OBJETO_H

#include "objeto.h"

class Objeto{
public:
    int id;
    int ancho;
    int alto;
    int x;
    int y;
    bool rotacion;

    Objeto(int ide, int anc, int alt);
    Objeto(int ide, int anc, int alt, int xx, int yy);
    int right() const;
    int bottom() const;
    bool getRotacion();
    int getAncho();
    int getAlto();
    int getId();
    void setAncho(int anc);
    void setAlto(int anc);
    void rotar();
};

#endif
