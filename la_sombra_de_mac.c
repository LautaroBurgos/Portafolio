#include "la_sombra_de_mac.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define MAC 'M'
#define BLOO 'B'
#define ARRIBA 'W'
#define ABAJO 'S'
#define IZQUIERDA 'A'
#define DERECHA 'D'
#define CESPED '~'
#define PUERTA 'D'
#define ESCALERA 'E'
#define LLAVE 'L'
#define MONEDA 'C'
#define POZO 'W'
#define INTERRUPTOR 'O'
#define PORTAL 'P'
#define VELA 'V'
#define PASITO 1
#define PUNTOS_INICIALES 0
#define VIDAS_INICIALES 3
#define ELEMENTO_NO_ENCONTRADO -1
#define	AMBOS '2'

bool coordenadas_iguales(coordenada_t a, coordenada_t b){
	return((a.col == b.col) && (a.fila == b.fila)) ;
}
void inicializar_personaje(personaje_t* ref_personaje, coordenada_t arranque_personaje){
	(*ref_personaje).puntos= PUNTOS_INICIALES;
	(*ref_personaje).vida= VIDAS_INICIALES;
	(*ref_personaje).tiene_llave = false;
	(*ref_personaje).interruptor_apretado=false;
	(*ref_personaje).posicion= arranque_personaje;
}
void inicializar_sombra(sombra_t* ref_sombra, coordenada_t arranque_sombra){
	(*ref_sombra).posicion= arranque_sombra;
	(*ref_sombra).esta_viva = true;
}
bool es_movimiento_valido(char movimiento){
	return((movimiento==ARRIBA)||(movimiento==ABAJO)||(movimiento==IZQUIERDA)||(movimiento==DERECHA));
}
void pedir_movimiento(char* ref_movimiento){	
	scanf(" %c", ref_movimiento);
	while(!es_movimiento_valido(*ref_movimiento)){
		scanf("%c",ref_movimiento);
	}
}
bool esta_dentro_rango(coordenada_t posicion, int max_alto, int max_ancho){	
	return (((posicion.col< max_ancho-1) &&(posicion.col>0))&&((posicion.fila< max_alto-1)&&(posicion.fila>0)));
}

void mover_personaje(personaje_t* ref_personaje, sombra_t* ref_sombra, char movimiento){
	switch(movimiento){
		case IZQUIERDA:
			//if((*ref_personaje).posicion.col>0){//Estas iteraciones antes de cada movimiento comprueban que no se escuentre en el extremo del terreno
			if(esta_dentro_rango((*ref_personaje).posicion,MAX_FILAS,MAX_COLUMNAS)){		
				(*ref_personaje).posicion.col -=PASITO;
			}
			if(esta_dentro_rango((*ref_sombra).posicion,MAX_FILAS,MAX_COLUMNAS)){
				(*ref_sombra).posicion.col+=PASITO ;
			}
			break;
		case ARRIBA:
			if(esta_dentro_rango((*ref_personaje).posicion,MAX_FILAS,MAX_COLUMNAS)){
				(*ref_personaje).posicion.fila -=PASITO;
			}
			if(esta_dentro_rango((*ref_sombra).posicion,MAX_FILAS,MAX_COLUMNAS)){
				(*ref_sombra).posicion.fila -=PASITO ;
			}
			break;
		case ABAJO:
			if(esta_dentro_rango((*ref_personaje).posicion,MAX_FILAS,MAX_COLUMNAS)){
				(*ref_personaje).posicion.fila +=PASITO;
			}	
			if(esta_dentro_rango((*ref_sombra).posicion,MAX_FILAS,MAX_COLUMNAS)){
				(*ref_sombra).posicion.fila +=PASITO ;
			}
			break;
		case DERECHA:
			if(esta_dentro_rango((*ref_personaje).posicion,MAX_FILAS,MAX_COLUMNAS)){
				(*ref_personaje).posicion.col +=PASITO;
			}
			if(esta_dentro_rango((*ref_sombra).posicion,MAX_FILAS,MAX_COLUMNAS)){
				(*ref_sombra).posicion.col -=PASITO;
			}
		break;
	}
}
coordenada_t coordenada_aleatoria(int max_alto, int max_ancho){
	coordenada_t coordenada_nueva;
	coordenada_nueva.fila= rand()% max_ancho;
	coordenada_nueva.col = rand()% max_alto;
	return coordenada_nueva;
}
int buscar_elemento(elemento_t elementos[MAX_ELEMENTOS], int tope, coordenada_t posicion){
	int retorno=ELEMENTO_NO_ENCONTRADO;
	int i=0;
	while((!coordenadas_iguales(elementos[i].posicion,posicion))&&(i<tope)){
		retorno = i;
		i++;
	}
	return retorno;
}
/*
	Funcion que va a devolver el caracter que corresponda para imprimir
	dependiendo de su posicion
*/
char decidir_que_imprimo(int fila,int columna,juego_t juego){
	if((fila==juego.personaje.posicion.fila)&&(columna==juego.personaje.posicion.col)){
		return MAC;
	}
	else if((fila==juego.sombra.posicion.fila)&&(columna==juego.sombra.posicion.col)){
		return BLOO;
	}
	else{
		return CESPED;
	}

}
void imprimir_terreno(juego_t juego){
	char terreno[MAX_FILAS][MAX_COLUMNAS];
	for (int i = 0; i < MAX_FILAS; i++){
		for (int j =0; j < MAX_COLUMNAS; j++)
		{		
			if(!coordenadas_iguales(juego.personaje.posicion,juego.sombra.posicion)){
				terreno[i][j]=decidir_que_imprimo(i,j,juego);
			}
			else{
				terreno[i][j]= decidir_que_imprimo(i,j,juego);
			}
		}
	}
	for (int i = 0; i < MAX_FILAS;i++){
		for(int j=0;j<MAX_COLUMNAS;j++){
			printf("-%c-", terreno[i][j]);
		}
		printf("\n");
	}
}