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
#include "individuo.h"
#include "solver.h"

/**
Parametros utilizados
ruta a la instancia
n iteraciones
tamaño poblacionActual
Imprimir o no, 0 = no, 1 = si
probabilidad de cruzamiento
probabilidad de mutacion
**/

int main(int args, char **argv){
    if(args!=7){
        std::cout<< "Parámetros Incorrectos";
        return 1;
    }

    Solver solv = Solver(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atof(argv[5]), atof(argv[6]));
    solv.leerInstancia(argv[1]);
    solv.algoritmoGenetico();
    if(solv.escribirOutput(argv[1]) == 1){
        std::cout << "Error al escribir el archivo \n";
    }
    return 0;
}
