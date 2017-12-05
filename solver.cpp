#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <list>
#include <map>
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

        int getId(){
            return this->id;
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
                randomNumber = ((double)std::rand() /RAND_MAX);
                if(randomNumber <= pmut){
                    i.rotar();
                }
            }
        }

        void BLF(){
            return;
        }

        void BL(){
            return;
        }
};

class Solver{
    public:
        int cantidadItem;
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
            this->cantidadItem = entrada1;
            archivo >> entrada1;

            this->anchoStrip = entrada1;

            while(archivo >> entrada1 >> entrada2 >> entrada3){
                Objeto obj(entrada1, entrada2, entrada3);
                this->items.addObjeto(obj);
            }

            return 0;
        }

        void generarPoblacion(){
            std::random_device rd;
            std::mt19937 g(rd());
            for(int i = 0; i < tamanoPoblacion; i++){
                Individuo aux = items;
                std::shuffle(aux.ordenObjetos.begin(), aux.ordenObjetos.end(),g);
                poblacionActual.push_back(aux);
            }
            return;
        }

        void seleccionarPadres(){
            return;
        }

        //Usando OrderBased-Crossover!!
        void cruzar(Individuo padre1, Individuo padre2){
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
            //Generar la población Inicial
            generarPoblacion();
            //Evaluar la población
            evaluarPoblacionActual();
            for(int i = 0; i < cantidadIter; i++){
                //Aplicamos el operador de selección
                seleccionarPadres();
                //Poblacion de tamaño par
                if(tamanoPoblacion%2 ==0){
                    for(int j=0; j < tamanoPoblacion; j += 2){
                        cruzar(poblacionActual[j], poblacionActual[j+1]);
                    }
                    for(auto &i : proxPoblacion){
                        i.mutar(getProbMutacion());
                    }
                }
                //Poblacion de tamaño impar
                else{
                    for(int j=0; j < (tamanoPoblacion - 1); j += 2){
                        cruzar(poblacionActual[j], poblacionActual[j+1]);
                    }
                    for(auto &i : proxPoblacion){
                        i.mutar(getProbMutacion());
                    }
                    //Acá se debe ingresar la mejor solución encontrada
                    proxPoblacion.push_back(poblacionActual[1]);
                }
                //Evaluamos nuestra nueva población
                evaluarProxPoblacion();
                this->poblacionActual = this->proxPoblacion;
                this->proxPoblacion.clear();
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

    Solver solv = Solver(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atof(argv[5]), atof(argv[6]));
    solv.leerInstancia(argv[1]);
    solv.algoritmoGenetico();
    return 0;
}
