#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <list>
#include <vector>
#include <random>

class Objeto{
    public:
        int id;
        int ancho;
        int alto;
        bool rotacion;

        Objeto(int ide, int anc, int alt){
            this->id = ide;
            this->ancho = anc;
            this->alto = alt;
            this->rotacion = false;
        }

        bool getRotacion(){
            return this->rotacion;
        }

        int getAncho(){
            return this->ancho;
        }

        int getAlto(){
            return this->alto;
        }

        void setAncho(int anc){
            this->ancho = anc;
        }

        void setAlto(int alt){
            this->alto = alt;
        }

        void rotar(){
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
};

class Individuo{
    public:
        std::vector<Objeto> ordenObjetos;
        int calidad;

        Individuo(){
            calidad = 10000000;
            ordenObjetos = std::vector<Objeto>();
        }

        void addObjeto(Objeto obj){
            ordenObjetos.push_back(obj);
        }

        void mutar(float pmut){
            double randomNumber;
            for(auto &i : this->ordenObjetos){
                randomNumber = ((double) std::rand() /RAND_MAX);
                std::cout << "Estamos en el for " << randomNumber ;
                if(randomNumber <= pmut){
                    i.rotar();
                }
            }
        }

        void BLF(){
            return;
        }
};

class Solver{
    public:
        int cantidadIter;
        int tamanoPoblacion;
        int opCruzamiento;
        int anchoStrip;
        float probCruzamiento;
        float probMutacion;
        Individuo items;
        Individuo bestInd;
        std::vector<Individuo> poblacionActual;
        std::vector<Individuo> padres;
        std::vector<Individuo> proxPoblacion;

        Solver(int iter, int tamano, int cruz, float pcruz, float pmut){
            this->cantidadIter = iter;
            this->tamanoPoblacion = tamano;
            this->opCruzamiento = cruz;
            this->probCruzamiento = pcruz;
            this->probMutacion = pmut;
            this->anchoStrip = 0;
            items = Individuo();
            poblacionActual = std::vector<Individuo>();
            proxPoblacion = std::vector<Individuo>();
        }

        int leerInstancia(std::string instancia){
            int entrada1, entrada2, entrada3;
            std::ifstream archivo(instancia);

            if(!archivo.good()){
                return 1;
            }

            archivo >> entrada1;
            archivo >> entrada1;

            this->anchoStrip = entrada1;

            while(archivo >> entrada1 >> entrada2 >> entrada3){
                Objeto obj{entrada1, entrada2, entrada3};
                this->items.addObjeto(obj);
            }

            return 0;
        }

        void generarPoblacion(){
            return;
        }

        void seleccionarPadres(){
            return;
        }

        void cruzar(Individuo padre1, Individuo padre2){
            return;
        }

        void evaluarPoblacionActual(){
            return;
        }

        void evaluarProxPoblacion(){
            return;
        }

        float getProbMutacion(){
            return this->probMutacion;
        }

        float getProbCruzamiento(){
            return this->probCruzamiento;
        }

        Individuo algoritmoGenetico(){
            generarPoblacion();
            evaluarPoblacionActual();
            for(int i = 0; i < cantidadIter; i++){
                seleccionarPadres();
                for(int j=0; j < tamanoPoblacion; j += 2){
                    //cruzar(padres[j], padres[j+1]);
                }
                for(auto &i : proxPoblacion){
                    i.mutar(getProbMutacion());
                }
                evaluarProxPoblacion();
                //this->poblacionActual = this->proxPoblacion;
            }
            return this->bestInd;
        }

};

/**
Parametros utilizados
ruta a la instancia
n iteraciones
tamaño poblacionActual
operador de cruzamiento
probabilidad de cruzamiento
probabilidad de mutacion
**/

int main(int args, char **argv){
    if(args!=7){
        std::cout<< "Parámetros Incorrectos";
        return 1;
    }

    Solver solv = Solver{atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atof(argv[5]), atof(argv[6])};
    solv.leerInstancia(argv[1]);
    solv.algoritmoGenetico();
    return 0;
}
