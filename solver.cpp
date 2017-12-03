#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <list>

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
        std::list<Objeto> ordenObjetos;
        int calidad;

        Individuo(){
            calidad = 10000000;
            ordenObjetos = std::list<Objeto>();        }

        void addObjeto(Objeto obj){
            ordenObjetos.push_back(obj);
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
        std::list<Individuo> poblacionActual;

        Solver(int iter, int tamano, int cruz, float pcruz, float pmut){
            this->cantidadIter = iter;
            this->tamanoPoblacion = tamano;
            this->opCruzamiento = cruz;
            this->probCruzamiento = pcruz;
            this->probMutacion = pmut;
            this->anchoStrip = 0;
            items = Individuo();
            poblacionActual = std::list<Individuo>();
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
                Objeto obj(entrada1, entrada2, entrada3);
                this->items.addObjeto(obj);
            }

            return 0;
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

    Solver solv = Solver(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atof(argv[5]), atof(argv[6]));
    solv.leerInstancia(argv[1]);
    return 0;
}
