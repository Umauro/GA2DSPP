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
//Estructura utilizada por BLF



Solver::Solver(int iter, int tamano, int prints, float pcruz, float pmut){
//Parámetros del algoritmo
    this->cantidadIter = iter;
    this->tamanoPoblacion = tamano;
    this->print = prints;
    this->probCruzamiento = pcruz;
    this->probMutacion = pmut;
    this->anchoStrip = 0;
    this->maxAltura = 0;
    this->areaObjetos = 0;
    //Parametros de las Instancia
    items = Individuo();
    poblacionActual = std::vector<Individuo>();
    proxPoblacion = std::vector<Individuo>();
}

int Solver::leerInstancia(std::string instancia){
    int entrada1, entrada2, entrada3;
    int maxAncho = 0;
    std::ifstream archivo(instancia);

    if(!archivo.good()){
        std::cout << "Error al leer el archivo \n";
        return 1;
    }

    archivo >> entrada1;
    this->cantidadItem = entrada1;
    archivo >> entrada1;

    this->anchoStrip = entrada1;

    while(archivo >> entrada1 >> entrada2 >> entrada3){
        Objeto obj(entrada1, entrada2, entrada3);
        this->items.addObjeto(obj);
        maxAltura += entrada3;
        maxAncho += entrada2;
        areaObjetos += entrada2 * entrada3;
    }
    if(maxAncho > maxAltura){
        maxAltura = maxAncho;
    }
    return 0;
}
        //Generador de población aleatoria
void Solver::generarPoblacion(){
    std::random_device rd;
    std::mt19937 g(rd());
    for(int i = 0; i < tamanoPoblacion; i++){
        Individuo aux = items;
        std::shuffle(aux.ordenObjetos.begin(), aux.ordenObjetos.end(),g);
        poblacionActual.push_back(aux);
    }
    return;
}

//Operador de selección utilizando 2-torneo
void Solver::seleccionarPadres(){
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(0,tamanoPoblacion-1);
    for(int i=0; i < tamanoPoblacion; i++){
        Individuo primero;
        Individuo segundo;
        primero = poblacionActual[uni(rng)];
        segundo = poblacionActual[uni(rng)];
        if(primero.getCalidad()> segundo.getCalidad()){
            padres.push_back(segundo);
        }
        else{
            padres.push_back(primero);
        }
    }
    return;
}

//Usando OrderBased-Crossover!!
void Solver::cruzar(Individuo padre1, Individuo padre2){
    double randomNumber;
    randomNumber = ((double)std::rand() /RAND_MAX);

    //Si se realiza cruzamiento o no

    if(randomNumber < probCruzamiento){
        //Se crean los hijos
        Individuo hijo1 = Individuo();
        Individuo hijo2 = Individuo();

        hijo1.ordenObjetos.resize(padre1.ordenObjetos.size(), Objeto(1,1,1));
        hijo2.ordenObjetos.resize(padre1.ordenObjetos.size(), Objeto(1,1,1));

        //Generador de Cortes Aleatorios
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> uni(1,cantidadItem-1);

        int corte1 = uni(rng);
        int corte2 = uni(rng);
        //Revisar que no se genere el mismo punto para el corte
        while(corte1 == 0 || corte1 == (cantidadItem-1)){
            corte1 = uni(rng);
        }
        while(corte2 == 0 || corte2 == (cantidadItem-1)){
            corte2 = uni(rng);
        }
        while(corte1 == corte2){
            corte2 = uni(rng);
        }

        //COMIENZA EL CRUZAMIENTOOOOOO!!!
        if(corte1 > corte2){
            int auxiliar = corte1;
            corte1 = corte2;
            corte2 = auxiliar;
        }
        //Creamos un Diccionario para ver si se agregó algo o no
        std::map<int,int> hashPadre1;
        std::map<int,int> hashPadre2;

        //Se agrega a los hijos lo que esté entre los cortes
        for(int i = corte1; i <= corte2; i++){
            Objeto obj1 = padre1.ordenObjetos[i];
            Objeto obj2 = padre2.ordenObjetos[i];

            //Guardamos en un Hash para ver que items se agregaron
            hashPadre1[obj1.getId()] = obj1.getId();
            hashPadre2[obj2.getId()] = obj2.getId();

            //Se hereda el cromosoma!
            hijo1.ordenObjetos[i] = obj1;
            hijo2.ordenObjetos[i] = obj2;

        }
        int i = 0;
        int j = 0;

        //Rellenamos la primera parte para el primer hijo
        while(corte1 - i > 0){
            std::map<int,int>::iterator it;
            Objeto aux = padre2.ordenObjetos[j];

            it = hashPadre1.find(aux.getId());
            if(it == hashPadre1.end()){
                hashPadre1[aux.getId()] = aux.getId();
                hijo1.ordenObjetos[i] = aux;
                i++;
                j++;
            }
            else{
                j++;
            }
        }

        i=0;
        int k = 0;

        //Rellenamos la primera parte para el segundo hijo
        while(corte1 - i > 0){
            std::map<int,int>::iterator it;
            Objeto aux = padre1.ordenObjetos[k];
            it = hashPadre2.find(aux.getId());
            if(it == hashPadre2.end()){
                hashPadre2[aux.getId()] = aux.getId();
                hijo2.ordenObjetos[i] = aux;
                i++;
                k++;
            }
            else{
                k++;
            }
        }

        i = corte2 +1;

        //Relleno de la segunda parte para el hijo 1
        while(cantidadItem - i > 0){
            std::map<int,int>::iterator it;
            Objeto aux = padre2.ordenObjetos[j];
            it = hashPadre1.find(aux.getId());
            if(it == hashPadre1.end()){
                hashPadre1[aux.getId()] = aux.getId();
                hijo1.ordenObjetos[i] = aux;
                i++;
                j++;
            }
            else{
                j++;
            }
        }

        i = corte2 +1;
        //Relleno de la primera parte para el hijo 2
        while(cantidadItem - i > 0){
            std::map<int,int>::iterator it;
            Objeto aux = padre1.ordenObjetos[k];
            it = hashPadre2.find(aux.getId());
            if(it == hashPadre2.end()){
                hashPadre2[aux.getId()] = aux.getId();
                hijo2.ordenObjetos[i] = aux;
                i++;
                k++;
            }
            else{
                k++;
            }
        }
        proxPoblacion.push_back(hijo1);
        proxPoblacion.push_back(hijo2);
    }
    else{
        proxPoblacion.push_back(padre1);
        proxPoblacion.push_back(padre2);
    }
    return;
}

void Solver::evaluarPoblacionActual(int anchotira, int altotira){
    int mejorCalidad = 1000000000;
    Individuo mejorIndividuo;
    for(auto &i : poblacionActual){
        i.BLF(anchotira, altotira);
        if(i.getCalidad()< mejorCalidad){
            mejorCalidad = i.getCalidad();
            mejorIndividuo = i;
            this->bestInd = mejorIndividuo;
        }
    }

    return;
}

void Solver::evaluarProxPoblacion(int anchotira,int altotira){
    Individuo mejorIndividuo;
    for(auto &i : proxPoblacion){
        i.BLF(anchotira, altotira);
        if(i.getCalidad()< bestInd.calidad){
            mejorIndividuo = i;
            this->bestInd = mejorIndividuo;
        }
    }

    return;
}

float Solver::getProbMutacion(){
    return this->probMutacion;
}

float Solver::getProbCruzamiento(){
    return this->probCruzamiento;
}

int Solver::escribirOutput(std::string ruta){
    std::string delimitador1 = "/";
    std::string delimitador2 = ".";
    std::string instancia = ruta.substr(0,ruta.find(delimitador2));
    instancia = instancia.substr(instancia.find(delimitador1)+1);
    instancia.append(".OUTPUT");
    std::ofstream archivo{instancia, std::ofstream::out};
    int desperdicio = (bestInd.calidad * anchoStrip) - areaObjetos;

    if (!archivo.good()){
        std::cout << "No se pudo abrir el archivo";
        return 1;
    }

    archivo << bestInd.calidad << "\n";
    archivo << desperdicio << "\n";

    for(auto &i : bestInd.ordenObjetos){
        archivo << i.x << " " << i.y << " " << i.rotacion << " "<< i.id << "\n";
    }
    archivo.close();
    return 0;
}

Individuo Solver::algoritmoGenetico(){
    int contadorCov = 0;
    int calidadActual = 10000000;
    //Generar la población Inicial
    generarPoblacion();
    //Evaluar la población
    evaluarPoblacionActual(anchoStrip, maxAltura);
    for(int i = 0; i < cantidadIter; i++){
        //Aplicamos el operador de selección
        seleccionarPadres();

        //Poblacion de tamaño par
        if(tamanoPoblacion%2 ==0){
            for(int j=0; j < tamanoPoblacion; j += 2){
                cruzar(padres[j], padres[j+1]);
            }
        }
        //Poblacion de tamaño impar
        else{
            for(int j=0; j < (tamanoPoblacion - 1); j += 2){
                cruzar(padres[j], padres[j+1]);
            }
            //Acá se debe ingresar la mejor solución encontrada
            proxPoblacion.push_back(bestInd);
        }
        //Aplicamos el operador de mutación
        for(auto &k : proxPoblacion){
            k.mutar(getProbMutacion(), anchoStrip);
        }
        //Evaluamos nuestra nueva población
        this->padres.clear();
        evaluarProxPoblacion(anchoStrip, maxAltura);

        if(calidadActual == bestInd.calidad){
            contadorCov += 1;
        }
        else if(calidadActual > bestInd.calidad){
            calidadActual = bestInd.calidad;
            contadorCov = 0;
        }

        this->poblacionActual = this->proxPoblacion;
        this->proxPoblacion.clear();

        //Si no cambia la solución en 3000 iteraciones se detiene el loop
        if(contadorCov == 3000){
            //std::cout<< i << "\n";
            break;
        }
    }
    if(print){
        this->bestInd.print(this->bestInd.ordenObjetos, anchoStrip, maxAltura);
    }
    std::cout << bestInd.calidad << "\n";
    return this->bestInd;
}
