# GA2DSPP
Algoritmo Genético + BLF para resolver el 2D Strip-Packing Problem

## Instrucciones de Compilación

Para compilar el programa, ingresar a la carpeta contenedora y ejecutar el
comando **make**

## Instrucciones de Ejecución

Una vez compilado, para ejecutar el programa se debe utilizar el siguiente
comando

~~~
make run INSTANCIA=[ruta a la instancia] POBLACION=[cantidad de poblacion] PRINT=[0 o 1] PCRUZ=[Float entre 0 y 1] PMUT=[Float entre 0 y 1]
~~~

Al ejecutar solo **make run** se utilizará la instancia HT01 presente en la carpeta instancias, se utilizará una población de 100 individuos, no se printeara la organización de los objetos en la superficie (1 para printear, 0 para no hacerlo) y se utilizará una probabilidad de 0.9 y 0.3 para el operador de cruzamiento y mutación respectivamente.

## Formato instancias

El formato de las instancias debe ser de la siguiente manera; La primera línea contendrá el número de objetos a empacar, la segunda línea contiene el ancho de la superficie, las siguientes líneas contienen los datos de cada objeto, colocando su id, su ancho y su largo. **IMPORTANTE:** Las instancias utilizadas no pueden contener objetos con un ancho mayor al ancho de la superficie.

**EJEMPLO**
~~~
5 //Cantidad de Objetos
10 //Ancho superficie
1 5 1 //id, ancho y largo del objeto
2 5 1
3 2 3
4 6 7
5 1 2
~~~
Por temas de la creación del archivo de OUTPUT, las instancias a utilizar se deben guardar en la carpeta **instancias**.

## Formato output

El output del programa se guarda en un archivo del tipo [Nombre Instancia].OUTPUT, el programa tiene el siguiente formato; Primero se entrega la altura que se utiliza, segundo se entrega el área desperdiciada por la solución. Por último se entregan 4 números por objeto: Coordenadas x e y de la esquina inferior izquierda, si el objeto se rotó o no (1 sí, 0 no) y el id del objeto.

**EJEMPLO**
~~~
20 //Altura obtenida
10 //Área desperdiciada
0 0 1 2 //Coordenada x, Coordenada y, Rotación?, id de objeto
3 0 1 1
0 7 1 7
0 11 1 4
6 0 1 9
0 13 0 6
8 0 1 10
8 9 0 5
0 18 1 3
4 13 0 8
~~~
