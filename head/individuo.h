#ifndef INDIVIDUO_H
#define INDIVIDUO_H
#include <vector>
#include <algorithm>
#include <random>
#include <set>
#include <iostream>

#include "objeto.h"
#include "individuo.h"

struct PositionComparator;

class Individuo{
public:
    std::vector<Objeto> ordenObjetos;
    int calidad;

    Individuo();
    void setCalidad(int cal);
    void addObjeto(Objeto obj);
    int getCalidad();
    void mutar(float pmut, int anchotira);
    void BLF(int anchotira, int altotira);
    void print(const std::vector<Objeto> & rects, int anchotira, int altotira);
};

#endif
