#include "la_sombra_de_mac.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define JUEGO_TERMINADO 'Z'
#define ELEMENTO_NO_ENCONTRADO -1
#define CORAZON "\u2764"
/*
	PRE:
* 	POST: carga un vector de elementos con una coordenada cualquiera para 
* probar el buscar_elemento
*/
void cargar_vector(elemento_t elementos[MAX_ELEMENTOS],int* ref_tope){
	(*ref_tope)=0;
	elementos[0].posicion.col= 4;
	elementos[0].posicion.fila=6;
	(*ref_tope)++;
}
/*
	PRE:
	POST:le asigna su correspondiente valor de fila y columna a la coordenada arranque_sombra
*/
void determinar_arranque_sombra(personaje_t personaje,coordenada_t* ref_arranque_sombra){
	(*ref_arranque_sombra).fila = personaje.posicion.fila;
	(*ref_arranque_sombra).col =(MAX_COLUMNAS-1)- (personaje.posicion.col); 
	}
/*
	PRE:
 *	POST:imprime los datos de Mac y de Bloo (posiciones,vidas de Mac,Bloo esta vivo o no,etc)
 */		
void imprimir_datos_juego(personaje_t personaje,sombra_t sombra){
	printf("VIDAS DE MAC: ");
	for (int i = 0; i < personaje.vida;i++){
		printf("%s",CORAZON );
	}
	printf("\n");
	printf("PUNTOS DE MAC: %i \n",personaje.puntos);
	if(personaje.tiene_llave==true){
		printf("MAC TIENE LA LLAVE\n");
	}
	else{
		printf("MAC NO TIENE LA LLAVE\n");
	}
	if(sombra.esta_viva){
		printf("BLOO ESTA VIVO\n");
	}
	else{
		printf("BLOO NO ESTA VIVO\n");
	}
}
/*
	PRE:
* 	POST:imprime el mensaje de juego terminado
*/
void imprimir_juego_terminado(){
			printf(" \
.......##.##.....##.########..######....#######.....########.########.########..##.....##.####.##....##....###....########...#######.\n \
.......##.##.....##.##.......##....##..##.....##.......##....##.......##.....##.###...###..##..###...##...##.##...##.....##.##.....##\n \
.......##.##.....##.##.......##........##.....##.......##....##.......##.....##.####.####..##..####..##..##...##..##.....##.##.....##\n \
.......##.##.....##.######...##...####.##.....##.......##....######...########..##.###.##..##..##.##.##.##.....##.##.....##.##.....##\n \
.##....##.##.....##.##.......##....##..##.....##.......##....##.......##...##...##.....##..##..##..####.#########.##.....##.##.....##\n \
.##....##.##.....##.##.......##....##..##.....##.......##....##.......##....##..##.....##..##..##...###.##.....##.##.....##.##.....##\n \
..######...#######..########..######....#######........##....########.##.....##.##.....##.####.##....##.##.....##.########...#######.\n");
}
/*
	PRE:
 * POST:actualiza,los datos del juego y se encarga de limpiar la pantalla
 */
void actualizar_datos(juego_t* ref_juego,personaje_t personaje,sombra_t sombra){
	(*ref_juego).personaje = personaje;
	(*ref_juego).sombra = sombra; 
	system("clear");
}

int main(){
	srand((unsigned) time(NULL));
	char movimiento;
	juego_t juego;
	personaje_t personaje;
	sombra_t sombra;
	coordenada_t arranque_personaje, arranque_sombra;

	elemento_t elementos[MAX_ELEMENTOS];
	int tope=0;
	cargar_vector(elementos,&tope);

	int max_alto,max_ancho;
	max_alto = MAX_FILAS;
	max_ancho = MAX_COLUMNAS;

	inicializar_personaje(&personaje,arranque_personaje);
	determinar_arranque_sombra(personaje,&arranque_sombra);
	inicializar_sombra(&sombra,arranque_sombra);
	actualizar_datos(&juego,personaje,sombra);
	imprimir_terreno(juego);
	imprimir_datos_juego(personaje,sombra);

	while (movimiento != JUEGO_TERMINADO){
		pedir_movimiento(&movimiento);
		if(es_movimiento_valido(movimiento)){
			if(esta_dentro_rango(personaje.posicion,max_alto,max_ancho) && esta_dentro_rango(sombra.posicion,max_alto,max_ancho)){
				mover_personaje(&personaje,&sombra,movimiento);
			}
		}
		else {
			if(movimiento== JUEGO_TERMINADO){
				imprimir_juego_terminado();
			}
			else{
				printf("El movimiento no es valido, ingrese otro movimiento\n");
			}
		}
		if(movimiento!=JUEGO_TERMINADO){
			actualizar_datos(&juego,personaje,sombra);
			imprimir_terreno(juego);
			imprimir_datos_juego(personaje,sombra);
			if (buscar_elemento(elementos,tope,personaje.posicion)==ELEMENTO_NO_ENCONTRADO){
				printf("ELEMENTO NO ENCONTRADO\n");
			}
			else{
				printf("ELEMENTO ENCONTRADO\n");
			}
		}
	}
return 0;
}