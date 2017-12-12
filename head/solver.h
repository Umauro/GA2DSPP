#ifndef SOLVER_H
#define SOLVER_H

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <random>

#include "objeto.h"
#include "individuo.h"
#include "solver.h"

class Solver{
public:
    int cantidadItem;
    int cantidadIter;
    int tamanoPoblacion;
    int print;
    int anchoStrip;
    float probCruzamiento;
    float probMutacion;
    Individuo items;
    Individuo bestInd;
    std::vector<Individuo> poblacionActual;
    std::vector<Individuo> padres;
    std::vector<Individuo> proxPoblacion;
    int maxAltura;
    int areaObjetos;

    Solver(int iter, int tamano, int prints, float pcruz, float pmut);
    int leerInstancia(std::string instancia);
    void generarPoblacion();
    void seleccionarPadres();
    void cruzar(Individuo padre1, Individuo padre2);
    void evaluarPoblacionActual(int anchotira, int altotira);
    void evaluarProxPoblacion(int anchotira, int altotira);
    float getProbMutacion();
    float getProbCruzamiento();
    int escribirOutput(std::string ruta);
    Individuo algoritmoGenetico();
};
#endif
