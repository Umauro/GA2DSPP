#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <list>
#include <map>
#include <set>
#include <vector>
#include <random>

class Objeto{
    public:
        int id;
        int ancho;
        int alto;
        int x;
        int y;
        bool rotacion;

        Objeto(int ide, int anc, int alt){
            this->id = ide;
            this->ancho = anc;
            this->alto = alt;
            this->rotacion = false;
        }
        //Sobrecarga de constructor para coordenadas
        Objeto(int ide, int anc, int alt, int x, int y){
            this->id = ide;
            this->ancho = anc;
            this->alto = alt;
            this->rotacion = false;
            this->x = x;
            this->y = y;
        }

        int right() const{
            return x + ancho - 1;
        }

        int bottom() const{
            return y + alto - 1;
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

//Estructura utilizada por BLF
struct PositionComparator{
    bool operator()(const Objeto a, Objeto b) const{
        return (a.y < b.y) ||
			   ((a.y == b.y) &&
				((a.x < b.x) ||
				 ((a.x == b.x) &&
				  ((a.alto > b.alto) ||
				   ((a.alto == b.alto) &&
					(a.ancho > b.ancho))))));

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

        void setCalidad(int calidad){
            this->calidad = calidad;
        }

        void addObjeto(Objeto obj){
            ordenObjetos.push_back(obj);
        }

        int getCalidad(){
            return this->calidad;
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

        void BLF(int anchotira, int altotira){
            int altura = 0;
            //Se crea conjunto para espacios disponibles
            std::set<Objeto, PositionComparator> gaps;
            //Al comienzo todo el strip está disponible
            gaps.insert(Objeto(-1,anchotira, altotira, 0 ,0));

            //Se comienza a colocar los objetos
            for(unsigned int i=0; i < ordenObjetos.size(); ++i){
                std::set<Objeto, PositionComparator>::iterator g;
                //Se busca por un espacio disponible para un objeto
                for(g = gaps.begin(); g!=gaps.end(); ++g){
                    if((g->ancho >= ordenObjetos[i].ancho) && (g->alto >=ordenObjetos[i].alto)){
                        break;
                    }
                }
                if(g != gaps.end()){
                    //Si se encuentra se asignan las coordenadas
                    ordenObjetos[i].x = g->x;
                    ordenObjetos[i].y = g->y;
                    //Se calcula que altura tendrá la solución al agregar este objeto
                    if(g->y + ordenObjetos[i].alto >= altura){
                        altura = g->y + ordenObjetos[i].alto;
                    }


                    //Se actualiza el conjunto de espacios disponibles
                    for(g = gaps.begin(); g!= gaps.end();){
                        if(!((ordenObjetos[i].right() < g->x) || (ordenObjetos[i].bottom() < g->y) ||
                             (ordenObjetos[i].x > g->right()) || (ordenObjetos[i].y > g->bottom()))){
                            //Se agrega el espacio disponible a la izquierda del objeto insertado
                            if(g->x < ordenObjetos[i].x){
                                gaps.insert(Objeto(-1, ordenObjetos[i].x - g->x, g->alto, g->x, g->y));
                            }
                            //Se agrega el espacio disponible a la arriba del objeto insertado
                            if(g->y < ordenObjetos[i].y){
                                gaps.insert(Objeto(-1,g->ancho,ordenObjetos[i].y - g->y, g->x, g->y));
                            }
                            //Se agrega el espacio disponible a la derecha del objeto insertado
                            if(g->right() > ordenObjetos[i].right()){
                                gaps.insert(Objeto(-1,g->right() - ordenObjetos[i].right(),g->alto,ordenObjetos[i].right()+1, g->y));
                            }
                            //Se agrega el espacio disponible a la abajo del objeto insertado
                            if(g->bottom() > ordenObjetos[i].bottom()){
                                gaps.insert(Objeto(-1,g->ancho, g->bottom()- ordenObjetos[i].bottom(),
                                                      g->x, ordenObjetos[i].bottom() + 1));
                            }
                            //Se eliminan intersecciones
                            gaps.erase(g++);
                        }
                        else{
                            ++g;
                        }
                    }
                }
            }
            setCalidad(altura);
            return;
        }

        void print(const std::vector<Objeto> & rects, int anchotira, int altotira){
            std::vector<int> image(anchotira*altotira,-1);
            // Print the Rects
            for (unsigned int i = 0; i < rects.size(); ++i) {
                if (rects[i].x != -1) {
                    for (int y = 0; y < rects[i].alto; ++y) {
                        for (int x = 0; x < rects[i].ancho; ++x) {
                            if (image[(rects[i].y + y) * anchotira + rects[i].x + x] == -1) {
                                image[(rects[i].y + y) * anchotira + rects[i].x + x] = i;
                            }
                            else {
                                std::cerr << "Error: two Rects overlap!" << std::endl;
                                exit(-1);
                            }
                        }
                    }
                }
                else {
                    std::cout << "Warning: a Rect os size " << rects[i].ancho << " x " << rects[i].alto
                         << " could not be packed." << std::endl;
                }
            }

            for (int x = 0; x < anchotira + 2; ++x)
                std::cout << '-';

            std::cout << std::endl;

            for (int y = 0; y < altotira; ++y) {
                std::cout << '|';

                for (int x = 0; x < anchotira; ++x) {
                    if (image[y * anchotira + x] != -1)
                        std::cout << static_cast<char>('A' + (image[y * anchotira + x] % 26));
                    else
                        std::cout << ' ';
                }

                std::cout << '|' << std::endl;
            }

            for (int x = 0; x < anchotira + 2; ++x)
                std::cout << '-';

            std::cout << std::endl;
        }
};

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

        Solver(int iter, int tamano, int print, float pcruz, float pmut){
            //Parámetros del algoritmo
            this->cantidadIter = iter;
            this->tamanoPoblacion = tamano;
            this->print = print;
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
                maxAltura += entrada3;
                areaObjetos += entrada2 * entrada3;

            }
            return 0;
        }
        //Generador de población aleatoria
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

        //Operador de selección utilizando 2-torneo
        void seleccionarPadres(){
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
                //std::cout << i << "\n";
            }
            //std::cout << "Seleccioné padres \n";
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

        void evaluarPoblacionActual(int anchotira, int altotira){
            int mejorCalidad = 1000000000;
            Individuo mejorIndividuo;
            for(auto &i : poblacionActual){
                i.BLF(anchotira, altotira);
                if(i.getCalidad()< mejorCalidad){
                    mejorCalidad = i.getCalidad();
                    mejorIndividuo = i;
                }
            }
            this->bestInd = mejorIndividuo;
            return;
        }

        void evaluarProxPoblacion(int anchotira,int altotira){
            int mejorCalidad = 1000000000;
            Individuo mejorIndividuo;
            for(auto &i : proxPoblacion){
                i.BLF(anchotira, altotira);
                if(i.getCalidad()< mejorCalidad){
                    mejorCalidad = i.getCalidad();
                    mejorIndividuo = i;
                }
            }
            this->bestInd = mejorIndividuo;
            return;
        }

        float getProbMutacion(){
            return this->probMutacion;
        }

        float getProbCruzamiento(){
            return this->probCruzamiento;
        }

        int escribirOutput(std::string ruta){
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
            std::cout << bestInd.calidad << "\n";

            archivo << bestInd.calidad << "\n";
            archivo << desperdicio << "\n";

            for(auto &i : bestInd.ordenObjetos){
                archivo << i.x << " " << i.y << " " << i.rotacion << "\n";
            }
            archivo.close();
            return 0;
        }

        Individuo algoritmoGenetico(){
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

                    for(auto &i : proxPoblacion){
                        i.mutar(getProbMutacion());
                    }

                }
                //Poblacion de tamaño impar
                else{
                    for(int j=0; j < (tamanoPoblacion - 1); j += 2){
                        cruzar(padres[j], padres[j+1]);
                    }
                    for(auto &i : proxPoblacion){
                        i.mutar(getProbMutacion());
                    }
                    //Acá se debe ingresar la mejor solución encontrada
                    proxPoblacion.push_back(bestInd);
                }
                //Evaluamos nuestra nueva población
                this->padres.clear();
                evaluarProxPoblacion(anchoStrip, maxAltura);
                this->poblacionActual = this->proxPoblacion;
                this->proxPoblacion.clear();
            }
            if(print){
                this->bestInd.print(this->bestInd.ordenObjetos, anchoStrip, maxAltura);
            }

            return this->bestInd;
        }

};

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
