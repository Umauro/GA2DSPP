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


Individuo::Individuo(){
    calidad = 10000000;
    ordenObjetos = std::vector<Objeto>();
}

void Individuo::setCalidad(int cal){
    this->calidad = cal;
}

void Individuo::addObjeto(Objeto obj){
    ordenObjetos.push_back(obj);
}

int Individuo::getCalidad(){
    return this->calidad;
}
void Individuo::mutar(float pmut, int anchotira){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> random(0,1.0);
    float randomNumber;
    for(auto &i : this->ordenObjetos){
        randomNumber = random(gen);
        if(randomNumber <= pmut){
            if(i.alto < anchotira){
                i.rotar();
            }
        }
    }
}

void Individuo::BLF(int anchotira, int altotira){
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

void Individuo::print(const std::vector<Objeto> & rects, int anchotira, int altotira){
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
