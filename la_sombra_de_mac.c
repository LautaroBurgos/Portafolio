#include "la_sombra_de_mac.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "utiles.h"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define MAC 'M'
#define BLOO 'B'
#define ARRIBA 'W'
#define ABAJO 'S'
#define IZQUIERDA 'A'
#define DERECHA 'D'
#define PEDIR_VIDA 'V'
#define CESPED '~'
#define PUERTA 'D'
#define ESCALERA 'E'
#define LLAVE 'L'
#define MONEDA 'C'
#define POZO 'W'
#define INTERRUPTOR 'O'
#define PORTAL 'P'
#define VELA 'V'
#define AREA_EFECTO_VELA '*'
#define PARED '|'

#define PASITO 1
#define PUNTOS_INICIALES 0
#define VIDAS_INICIALES 3
#define GANADO 1
#define PERDIDO -1
#define JUGANDO 0
#define PUERTA_POR_NIVEL 1
#define POS_NIVEL_1 0
#define POS_NIVEL_2 1
#define POS_NIVEL_3 2
#define NIVEL_1 1
#define NIVEL_2 2
#define NIVEL_3 3
#define MAX_VIDAS 3
#define PUNTOS_POR_VIDA 200
#define PUNTOS_POR_REVIVIR_A_BLOO 50

#define VELAS_NIVEL_1 5
#define POZOS_NIVEL_1 15
#define ESCALERAS_NIVEL_1 10
#define MONEDAS_NIVEL_1 10
#define PORTALES_NIVEL_1 0
#define INTERRUPTORES_NIVEL_1 1
#define LLAVES_NIVEL_1 0

#define VELAS_NIVEL_2 10
#define POZOS_NIVEL_2 20
#define ESCALERAS_NIVEL_2 15
#define MONEDAS_NIVEL_2 15
#define PORTALES_NIVEL_2 2
#define	INTERRUPTORES_NIVEL_2 2
#define LLAVES_NIVEL_2 1

#define VELAS_NIVEL_3 12
#define POZOS_NIVEL_3 30
#define ESCALERAS_NIVEL_3 15
#define MONEDAS_NIVEL_3 15
#define PORTALES_NIVEL_3 4
#define	INTERRUPTORES_NIVEL_3 4
#define LLAVES_NIVEL_3 1

#define ELEMENTO_NO_ENCONTRADO -1
#define	AMBOS '2'
/*
*    Función que recibe dos coordenadas, devuelve true si las
* coordenadas son iguales, sino false.
*/
bool coordenadas_iguales(coordenada_t a, coordenada_t b){
	return((a.col == b.col) && (a.fila == b.fila)) ;
}
/*
*    La función recibe un vector de elementos, su respectivo tope y una coordenada,
* devuelve la posición del elemento del vector, que coincida con la coordenada pasada 'posicion',
* si no se encuentra ningún elemento, se devolverá -1. 
*/
int buscar_elemento(elemento_t elementos[MAX_ELEMENTOS], int tope, coordenada_t posicion){
	int retorno=ELEMENTO_NO_ENCONTRADO;
	bool encontro=false;
	int i=0;
	while((i<tope)&&(!encontro)){
		if(coordenadas_iguales(elementos[i].coordenada,posicion)){
			retorno = i;
			encontro=true;
		}
		i++;
	}
	return retorno;
}
/*
	Funcion que chequea que ni MAC,ni BLOO empiezen el juego sobre alguna pared
*/
bool pueden_arrancar(coordenada_t paredes[MAX_PAREDES],int tope_paredes,coordenada_t arranque_personaje,coordenada_t arranque_sombra){
	bool mac_puede_arrancar=true;
	bool bloo_puede_arrancar=true;
	int i=0;
	while(mac_puede_arrancar&&i<tope_paredes){
		if(coordenadas_iguales(paredes[i],arranque_personaje)){
			mac_puede_arrancar=false;
		}
		i++;
	}
	i=0;
	if(mac_puede_arrancar){
		while(bloo_puede_arrancar&&i<tope_paredes){
			if(coordenadas_iguales(paredes[i],arranque_sombra)){
				bloo_puede_arrancar=false;
			}
			i++;
		}
	}	
	return (mac_puede_arrancar&&bloo_puede_arrancar);
}
/*
*   Función que dados una altura máxima y un ancho máximo
* devuelve una coordenada aleatoria. Una posición nunca podrá
* tener un valor negativo, ni ser mayor a sus máximos.
*/
coordenada_t coordenada_aleatoria(int max_alto, int max_ancho){
	coordenada_t coordenada_nueva;
	coordenada_nueva.fila= rand()% max_ancho;
	coordenada_nueva.col = rand()% max_alto;
	return coordenada_nueva;
}
/*
*   Procedimiento que dejará la estructura personaje_t en un
* estado válido para iniciar el juego.
* 'arranque_personaje' es un parámetro que representa la posición 
* en la arrancará el personaje.
*/
void inicializar_personaje(personaje_t* ref_personaje, coordenada_t arranque_personaje){
	(*ref_personaje).puntos= PUNTOS_INICIALES;
	(*ref_personaje).vida= VIDAS_INICIALES;
	(*ref_personaje).tiene_llave = false;
	(*ref_personaje).interruptor_apretado=false;
	(*ref_personaje).posicion= arranque_personaje;
}
/*
*   Procedimiento que dejará una estructura elemento_t que
* represente a la sombra, en un estado válido para iniciar el juego. 
* 'arranque_sombra' es un parámetro que representa la posición 
* en la arrancará la sombra.
*/
void inicializar_sombra(sombra_t* ref_sombra, coordenada_t arranque_sombra){
	(*ref_sombra).posicion= arranque_sombra;
	(*ref_sombra).esta_viva = true;
}
/*
* Funcion que calcula la distancia manhattan entre dos coordenadas y la devuelve
*/
int distancia_manhattan(coordenada_t a,coordenada_t b){
	int distancia_x=((a.col)-(b.col));
	int distancia_y=((a.fila)-(b.fila));
	if(distancia_x<0){
		distancia_x=distancia_x*(-1);
	}
	if(distancia_y<0){
		distancia_y=distancia_y*(-1);
	}
	return distancia_x+distancia_y;
}
/*
	 Funcion que revisa que ambos personajes esten a distancia manhattan <=1 de alguna puerta, devuelve true de ser asi
	 o false en caso contrario 
*/
bool estan_distancia_puerta(elemento_t herramientas[MAX_ELEMENTOS],int tope_herramientas,coordenada_t posicion_personaje,coordenada_t posicion_sombra){
	bool distancia_correcta=false;
	int i=0;
	while((i<tope_herramientas)&&(!distancia_correcta)){
		if(herramientas[i].tipo==PUERTA){
			if((distancia_manhattan(herramientas[i].coordenada,posicion_personaje)<=1)&&(distancia_manhattan(herramientas[i].coordenada,posicion_sombra)<=1)){
				distancia_correcta=true;
			}
		}
		i++;
	}
	return distancia_correcta;
}
/*
*   La función recibe un caracter y devuelve true
* si el movimiento recibido concuerda con la convención
* propuesta, sino false.

* -> W: Si el personaje debe moverse para la arriba. 
* -> A: Si el personaje debe moverse para la izquierda.
* -> S: Si el personaje debe moverse para la abajo.
* -> D: Si el personaje debe moverse para la derecha.
*/
bool es_movimiento_valido(char movimiento){
	if((movimiento==ARRIBA)||(movimiento==ABAJO)||(movimiento==IZQUIERDA)||(movimiento==DERECHA)){
		return true;
	}
	else{
		return false;
	}
}
/*
*   Procedimiento que se encargará de pedirle al usuario
* que ingrese un movimiento hasta que sea válido.(A,S,D,W O PEDIR VIDA)
*/
void pedir_movimiento(char* ref_movimiento){	
	scanf(" %c", ref_movimiento);
	if(((*ref_movimiento)!=PEDIR_VIDA)&&(!es_movimiento_valido(*ref_movimiento))){
		while(!es_movimiento_valido(*ref_movimiento)){
			scanf("%c",ref_movimiento);
		}
	}
}
/*
*    La función recibe una coordenada, devuelve true
* si la posición dentro de las dimensiones dadas, sino false.
* Una posición nunca podrá tener un valor negativo y no tendrá un 
* valor más alto que los máximos.
*/
bool esta_dentro_rango(coordenada_t posicion, int max_alto, int max_ancho){	
	return (((posicion.col<= max_ancho-1) &&(posicion.col>=0))&&((posicion.fila<= max_alto-1)&&(posicion.fila>=0)));
}
/*
	Funcion que revisa que una posicion este en el area de efecto de una vela , en direcciones diagonales al centro de la vela
*/
bool esta_en_esquinas(coordenada_t posicion,elemento_t vela){
	if((posicion.fila==vela.coordenada.fila-1)&&(posicion.col==vela.coordenada.col-1)){
		return true;
	}
	else if((posicion.fila==vela.coordenada.fila-1)&&(posicion.col==vela.coordenada.col+1)){
		return true;
	}
	else if((posicion.fila==vela.coordenada.fila+1)&&(posicion.col==vela.coordenada.col-1)){
		return true;
	}
	else if((posicion.fila==vela.coordenada.fila+1)&&(posicion.col==vela.coordenada.col+1)){
		return true;
	}
	else{
		return false;
	}
}
/*
	Funcion que dada una posicion, devuelve true si se encuentra en el  area de efecto de cualquier vela
*/
bool esta_en_rango_velas(coordenada_t posicion,elemento_t obstaculos[MAX_ELEMENTOS],int tope_obstaculos){
	elemento_t velas[MAX_ELEMENTOS];
	int i=0;
	int tope_velas=0;
	bool en_rango=false;
	for(int i=0;i<tope_obstaculos;i++){
		if(obstaculos[i].tipo==VELA){
			velas[tope_velas]=obstaculos[i];
			tope_velas++;
		}
	}
	while((i<tope_velas)&&(!en_rango)){
		if((distancia_manhattan(posicion,velas[i].coordenada)<=1)||(esta_en_esquinas(posicion,velas[i]))){
			en_rango=true;
		}
		i++;
	}
	return en_rango;
}
/*
	Procedimiento que se utiliza para guardar en una matriz de caractees el area de efecto de una vela (3x3)
*/
void crear_matriz_de_efecto_velas(char terreno[MAX_FILAS][MAX_COLUMNAS],elemento_t obstaculos[MAX_ELEMENTOS], int tope_obstaculos){
	elemento_t velas[MAX_ELEMENTOS];
	int tope_velas=0;
	for(int i=0;i<tope_obstaculos;i++){
		if(terreno[obstaculos[i].coordenada.fila][obstaculos[i].coordenada.col]==VELA){
			velas[tope_velas]=obstaculos[i];
			tope_velas++;
		}
	}
	for(int h=0;h<tope_velas;h++){
		for(int i=velas[h].coordenada.fila-1;i<velas[h].coordenada.fila+2;i++ ){
			for(int j=velas[h].coordenada.col-1;j<velas[h].coordenada.col+2;j++){
				if(terreno[i][j]==CESPED){
					terreno[i][j]=AREA_EFECTO_VELA;
				}
			}
		}
	}
}/*
	Funcion que revisa que en la siguiende posicion, dependiendo de hacia donde se mueva y considerando que hay una pared
	adelante si hay una escalera o no.(Como las escaleras son la unica herramienta que puede aparecer sobre una pared
	chequeo solamente si hay una herramienta o no sobre la siguiente posicion).

	PRE:El juego y los niveles ya deben estar inicializados.
	POST: Devuelve true si en la posicion siguiente, donde ya se aclaro que hay una pared, hay una escalera, o false
	en caso contrario.
*/
bool hay_escalera(elemento_t herramientas[MAX_ELEMENTOS],int tope_herramientas,coordenada_t posicion,char movimiento){
	coordenada_t posicion_siguiente;
	bool encontro=false;
	int i=0;
	switch(movimiento){
		case ARRIBA:
			posicion_siguiente.fila=posicion.fila-1;
			posicion_siguiente.col=posicion.col;
			while((i<tope_herramientas)&&(!encontro)){
				if(coordenadas_iguales(herramientas[i].coordenada,posicion_siguiente)){
					encontro=true;
				}
				i++;
			}
		break;
		case ABAJO:
			posicion_siguiente.fila=posicion.fila+1;
			posicion_siguiente.col=posicion.col;
			while((i<tope_herramientas)&&(!encontro)){
				if(coordenadas_iguales(herramientas[i].coordenada,posicion_siguiente)){
					encontro=true;
				}
				i++;
			}
		break;
		case IZQUIERDA:
			posicion_siguiente.fila=posicion.fila;
			posicion_siguiente.col=posicion.col-1;
			while((i<tope_herramientas)&&(!encontro)){
				if(coordenadas_iguales(herramientas[i].coordenada,posicion_siguiente)){
					encontro=true;
				}
				i++;
			}
		break;
		case DERECHA:
			posicion_siguiente.fila=posicion.fila;
			posicion_siguiente.col=posicion.col+1;
			while((i<tope_herramientas)&&(!encontro)){
				if(coordenadas_iguales(herramientas[i].coordenada,posicion_siguiente)){
					encontro=true;
				}
				i++;
			}
		break;
	}
	return encontro;
}
/*
	Funcion que revisa si en la posicion siguiente de un personaje hay una pared, devuelve true si la hay o false
	en caso contrario.
*/
bool hay_pared(coordenada_t paredes[MAX_PAREDES],int tope_paredes,coordenada_t posicion,char movimiento){
	coordenada_t posicion_siguiente;
	bool encontro=false;
	int i=0;
	switch(movimiento){
		case ARRIBA:
			posicion_siguiente.fila=posicion.fila-1;
			posicion_siguiente.col=posicion.col;
			while((i<tope_paredes)&&(!encontro)){
				if(coordenadas_iguales(paredes[i],posicion_siguiente)){
					encontro=true;
				}
				i++;
			}
		break;
		case ABAJO:
			posicion_siguiente.fila=posicion.fila+1;
			posicion_siguiente.col=posicion.col;
			while((i<tope_paredes)&&(!encontro)){
				if(coordenadas_iguales(paredes[i],posicion_siguiente)){
					encontro=true;
				}
				i++;
			}
		break;
		case IZQUIERDA:
			posicion_siguiente.fila=posicion.fila;
			posicion_siguiente.col=posicion.col-1;
			while((i<tope_paredes)&&(!encontro)){
				if(coordenadas_iguales(paredes[i],posicion_siguiente)){
					encontro=true;
				}
				i++;
			}
		break;
		case DERECHA:
			posicion_siguiente.fila=posicion.fila;
			posicion_siguiente.col=posicion.col+1;
			while((i<tope_paredes)&&(!encontro)){
				if(coordenadas_iguales(paredes[i],posicion_siguiente)){
					encontro=true;
				}
				i++;
			}
		break;
	}
	return encontro;
}
/*	Funcion recursiva que se llama en caso de cruzarse con una escalera, devuelve la cantidad de "PASOS", que va a dar MAC,
 dependiendo de cuantas escaleras seguidas detecte, o en caso de cruzarse con una pared luego de  X cantidad de escaleras
 devuelve 0 (pasos). O sea que no puede avanzar
*/
int paredes_y_escaleras_rec(elemento_t herramientas[MAX_ELEMENTOS],int tope_herramientas,coordenada_t paredes[MAX_PAREDES],int tope_paredes,char movimiento,coordenada_t posicion,int pasitos){
	bool termino=false;
	switch(movimiento){
		case ARRIBA:
			if(hay_pared(paredes,tope_paredes,posicion,movimiento)&&(hay_escalera(herramientas,tope_herramientas,posicion,movimiento))){
				pasitos++;
				posicion.fila-=1;
			}
			else if(hay_pared(paredes,tope_paredes,posicion,movimiento)&&(!hay_escalera(herramientas,tope_herramientas,posicion,movimiento))){
				pasitos= 0;
			}
			else{
				if(esta_dentro_rango(posicion,MAX_FILAS,MAX_COLUMNAS)){
					termino=true;
					pasitos++;
				}
				else{
					pasitos=0;
				}
			}
		break;
		case ABAJO:
			if(hay_pared(paredes,tope_paredes,posicion,movimiento)&&(hay_escalera(herramientas,tope_herramientas,posicion,movimiento))){
				pasitos++;
				posicion.fila+=1;
			}
			else if(hay_pared(paredes,tope_paredes,posicion,movimiento)&&(!hay_escalera(herramientas,tope_herramientas,posicion,movimiento))){
				pasitos= 0;
			}
			else{
				if(esta_dentro_rango(posicion,MAX_FILAS,MAX_COLUMNAS)){
					termino=true;
					pasitos++;
				}
				else{
					pasitos=0;
				}
			}	
		break;
		case DERECHA:
			if(hay_pared(paredes,tope_paredes,posicion,movimiento)&&(hay_escalera(herramientas,tope_herramientas,posicion,movimiento))){
				pasitos++;
				posicion.col+=1;
			}
			else if(hay_pared(paredes,tope_paredes,posicion,movimiento)&&(!hay_escalera(herramientas,tope_herramientas,posicion,movimiento))){
				pasitos= 0;
			}
			else{
				if(esta_dentro_rango(posicion,MAX_FILAS,MAX_COLUMNAS)){
					termino=true;
					pasitos++;
				}
				else{
					pasitos=0;
				}
			}
		break;
		case IZQUIERDA:
			if(hay_pared(paredes,tope_paredes,posicion,movimiento)&&(hay_escalera(herramientas,tope_herramientas,posicion,movimiento))){
				pasitos++;
				posicion.col-=1;
			}
			else if(hay_pared(paredes,tope_paredes,posicion,movimiento)&&(!hay_escalera(herramientas,tope_herramientas,posicion,movimiento))){
				pasitos= 0;
			}
			else{
				if(esta_dentro_rango(posicion,MAX_FILAS,MAX_COLUMNAS)){
					termino=true;
					pasitos++;
				}
				else{
					pasitos=0;
				}
			}
		break;
	}
	if(pasitos==0){
		return pasitos;
	}
	else if(termino){
		return pasitos;
	}
	else{
		return paredes_y_escaleras_rec(herramientas,tope_herramientas,paredes,tope_paredes,movimiento,posicion,pasitos);
	}
}
/* Funcion utilizada indirectamente para la funcion recursiva que recorre escaleras
*/
int paredes_y_escaleras(elemento_t herramientas[MAX_ELEMENTOS],int tope_herramientas,coordenada_t paredes[MAX_PAREDES],int tope_paredes,char movimiento,coordenada_t posicion){
	int pasitos=0;
	return paredes_y_escaleras_rec(herramientas,tope_herramientas,paredes,tope_paredes,movimiento,posicion,pasitos);
}
/*
*	Procedimiento que elimina un elemento de un vector de elementos ordenadamente
*/
void eliminar_elemento(elemento_t elementos[MAX_ELEMENTOS],int* tope,int posicion_buscado){
	if(posicion_buscado==ELEMENTO_NO_ENCONTRADO){
		return;
	}
	for(int i=posicion_buscado;i<(*tope-1);i++){
		elementos[i]=elementos[i+1];
	}
	(*tope)--;
}
/*	
	Funcion booleana que devuelve true si ya no quedan llaves en el vector de herramientas, caso contrario devuelve false;
*/
bool encontro_todas_llaves(elemento_t herramientas[MAX_ELEMENTOS],int tope_herramientas){
	bool encontro_todas=true;
	int i=0;
	while((encontro_todas)&&(i<tope_herramientas)){
		if(herramientas[i].tipo==LLAVE){
			encontro_todas=false;
		}
		i++;
	}
	return encontro_todas;
}
/* 
	Procedimiento que revisa que , en caso de cruzarse con algun obstaculo o herramienta realiza la accion correspondiente,
	y si es necesario lo elimina de su respectivo vector
*/
void chequear_obstaculos(juego_t* juego){
	int nivel=((*juego).nivel_actual-1);
	int posicion_comparada_mac_obs=(buscar_elemento((*juego).niveles[nivel].obstaculos,(*juego).niveles[nivel].tope_obstaculos,(*juego).personaje.posicion));
	int posicion_comparada_bloo_obs=(buscar_elemento((*juego).niveles[nivel].obstaculos,(*juego).niveles[nivel].tope_obstaculos,(*juego).sombra.posicion));
	int posicion_comparada_mac_herr=(buscar_elemento((*juego).niveles[nivel].herramientas,(*juego).niveles[nivel].tope_herramientas,(*juego).personaje.posicion));
	int posicion_comparada_bloo_herr=(buscar_elemento((*juego).niveles[nivel].herramientas,(*juego).niveles[nivel].tope_herramientas,(*juego).sombra.posicion));
	coordenada_t auxiliar;
	if(posicion_comparada_mac_obs!=ELEMENTO_NO_ENCONTRADO){
		if((*juego).niveles[nivel].obstaculos[posicion_comparada_mac_obs].tipo==POZO){
			(*juego).personaje.vida-=1;
			coordenada_t coordenada_nueva_mac=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
			coordenada_t coordenada_nueva_bloo;
			coordenada_nueva_bloo.fila=coordenada_nueva_mac.fila;
			coordenada_nueva_bloo.col=((MAX_COLUMNAS-coordenada_nueva_mac.col)-1);

			while(!pueden_arrancar((*juego).niveles[nivel].paredes,(*juego).niveles[nivel].tope_paredes,coordenada_nueva_mac,coordenada_nueva_bloo)
				||(coordenada_nueva_mac.fila>MAX_FILAS-1)||(coordenada_nueva_mac.col>MAX_COLUMNAS-1)){
				coordenada_nueva_mac=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				coordenada_nueva_bloo.fila=coordenada_nueva_mac.fila;
				coordenada_nueva_bloo.col=((MAX_COLUMNAS-coordenada_nueva_mac.col)-1);
			}
			(*juego).personaje.posicion=coordenada_nueva_mac;
			(*juego).sombra.posicion=coordenada_nueva_bloo;	
		}
		else if((*juego).niveles[nivel].obstaculos[posicion_comparada_mac_obs].tipo==INTERRUPTOR){
			if((*juego).personaje.interruptor_apretado){
				(*juego).personaje.interruptor_apretado=false;
			}
			else{
				(*juego).personaje.interruptor_apretado=true;
			}
		}
		else if((*juego).niveles[nivel].obstaculos[posicion_comparada_mac_obs].tipo==VELA){
			eliminar_elemento((*juego).niveles[nivel].obstaculos,(&(*juego).niveles[nivel].tope_obstaculos),posicion_comparada_mac_obs);
		}
	}
	if(posicion_comparada_bloo_obs!=ELEMENTO_NO_ENCONTRADO){
		if((*juego).niveles[nivel].obstaculos[posicion_comparada_bloo_obs].tipo==PORTAL){
			auxiliar=(*juego).personaje.posicion;
			(*juego).personaje.posicion=(*juego).sombra.posicion;
			(*juego).sombra.posicion=auxiliar;
		}
	}
	if(posicion_comparada_mac_herr!=ELEMENTO_NO_ENCONTRADO){
		if((*juego).niveles[nivel].herramientas[posicion_comparada_mac_herr].tipo==LLAVE){
			eliminar_elemento(((*juego).niveles[nivel].herramientas),(&(*juego).niveles[nivel].tope_herramientas),posicion_comparada_mac_herr);
			if(encontro_todas_llaves(((*juego).niveles[nivel].herramientas),(*juego).niveles[nivel].tope_herramientas)){
				(*juego).personaje.tiene_llave=true;
			}
		}
		else if((*juego).niveles[nivel].herramientas[posicion_comparada_mac_herr].tipo==MONEDA){
			(*juego).personaje.puntos+=rand()%11+10;
			eliminar_elemento(((*juego).niveles[nivel].herramientas),(&(*juego).niveles[nivel].tope_herramientas),posicion_comparada_mac_herr);
		}
		else if((*juego).niveles[nivel].herramientas[posicion_comparada_mac_herr].tipo==ESCALERA){
			eliminar_elemento(((*juego).niveles[nivel].herramientas),(&(*juego).niveles[nivel].tope_herramientas),posicion_comparada_mac_herr);
		}
	}
	if(posicion_comparada_bloo_herr!=ELEMENTO_NO_ENCONTRADO){
		if((*juego).niveles[nivel].herramientas[posicion_comparada_bloo_herr].tipo==MONEDA){
			(*juego).personaje.puntos+=rand()%11+10;
			eliminar_elemento(((*juego).niveles[nivel].herramientas),(&(*juego).niveles[nivel].tope_herramientas),posicion_comparada_bloo_herr);
		}
	}
	if(esta_en_rango_velas((*juego).sombra.posicion,(*juego).niveles[nivel].obstaculos,(*juego).niveles[nivel].tope_obstaculos)){
		(*juego).sombra.esta_viva=false;
	}
	if(coordenadas_iguales((*juego).personaje.posicion,(*juego).sombra.posicion)&&(!(*juego).sombra.esta_viva)){
		if(!esta_en_rango_velas((*juego).sombra.posicion,(*juego).niveles[nivel].obstaculos,(*juego).niveles[nivel].tope_obstaculos)){
			(*juego).sombra.esta_viva=true;
		}
		(*juego).personaje.puntos-=PUNTOS_POR_REVIVIR_A_BLOO;
	}
}
/*
*Procedimiento que dependiendo de las condiciones del juego permite o no mover a los personajes
*Y en el caso especifico de las escaleras,si puede usarlas , las elimina luego de hacerlo

* El personaje se mueve de la forma:
* -> W: Si el personaje debe moverse para la arriba. 
* -> A: Si el personaje debe moverse para la izquierda.
* -> S: Si el personaje debe moverse para la abajo.
* -> D: Si el personaje debe moverse para la derecha.
*
*   Si el interruptor no esta apretado la sombra se mueve de igual forma para el eje vertical, pero
* para el eje horizontal se moverá de forma inversa:
*
* -> A: Si el personaje debe moverse para la derecha.
* -> D: Si el personaje debe moverse para la izquierda.
	En cambio si el interruptor esta apretado, la sombra se mueve igual que el personaje
	->La sombra solo se puede mover si esta viva
*/
void mover_personajes_o_saltar_escaleras(juego_t* juego, char movimiento){
	int nivel=(*juego).nivel_actual-1;
	int pasos_a_dar;
	int pos_escalera_eliminar;
	char movimiento_espejo;
	if(movimiento==IZQUIERDA){
		movimiento_espejo=DERECHA;
	}
	else if(movimiento==DERECHA){
		movimiento_espejo=IZQUIERDA;
	}
	switch(movimiento){
		case IZQUIERDA:
			if(esta_dentro_rango((*juego).personaje.posicion,MAX_FILAS,MAX_COLUMNAS)&&((*juego).personaje.posicion.col!=0)){		
				if((!hay_pared((*juego).niveles[nivel].paredes,(*juego).niveles[nivel].tope_paredes,(*juego).personaje.posicion,movimiento))){	
					(*juego).personaje.posicion.col -=PASITO;
				}
				else{
					pasos_a_dar=(paredes_y_escaleras((*juego).niveles[nivel].herramientas,(*juego).niveles[nivel].tope_herramientas,(*juego).niveles[nivel].paredes,(*juego).niveles[nivel].tope_paredes,movimiento,(*juego).personaje.posicion));
					for(int i=0;i<pasos_a_dar;i++){
						(*juego).personaje.posicion.col-=PASITO;
						pos_escalera_eliminar=buscar_elemento((*juego).niveles[nivel].herramientas,(*juego).niveles[nivel].tope_herramientas,(*juego).personaje.posicion);
						eliminar_elemento((*juego).niveles[nivel].herramientas,(&(*juego).niveles[nivel].tope_herramientas),pos_escalera_eliminar);
					}
				}
			}
			if((*juego).sombra.esta_viva){
				if(!(*juego).personaje.interruptor_apretado){
					if(esta_dentro_rango((*juego).sombra.posicion,MAX_FILAS,MAX_COLUMNAS)&&(*juego).sombra.posicion.col!=MAX_COLUMNAS-1&&(!hay_pared((*juego).niveles[nivel].paredes,(*juego).niveles[nivel].tope_paredes,(*juego).sombra.posicion,movimiento_espejo))){
						(*juego).sombra.posicion.col+=PASITO;
					}
				}
				else{
					if(esta_dentro_rango((*juego).sombra.posicion,MAX_FILAS,MAX_COLUMNAS)&&((*juego).sombra.posicion.col!=0)&&(!hay_pared((*juego).niveles[nivel].paredes,(*juego).niveles[nivel].tope_paredes,(*juego).sombra.posicion,movimiento)) ){
						(*juego).sombra.posicion.col-=PASITO ;
					}
				}
			}
		break;
		case ARRIBA:
			if(esta_dentro_rango((*juego).personaje.posicion,MAX_FILAS,MAX_COLUMNAS)&&(*juego).personaje.posicion.fila!=0){
				if((!hay_pared((*juego).niveles[nivel].paredes,(*juego).niveles[nivel].tope_paredes,(*juego).personaje.posicion,movimiento))){	
					(*juego).personaje.posicion.fila -=PASITO;
				}
				else{
					pasos_a_dar=(paredes_y_escaleras((*juego).niveles[nivel].herramientas,(*juego).niveles[nivel].tope_herramientas,(*juego).niveles[nivel].paredes,(*juego).niveles[nivel].tope_paredes,movimiento,(*juego).personaje.posicion));
					for(int i=0;i<pasos_a_dar;i++){
						(*juego).personaje.posicion.fila -=PASITO;
						pos_escalera_eliminar=buscar_elemento((*juego).niveles[nivel].herramientas,(*juego).niveles[nivel].tope_herramientas,(*juego).personaje.posicion);
						eliminar_elemento((*juego).niveles[nivel].herramientas,(&(*juego).niveles[nivel].tope_herramientas),pos_escalera_eliminar);
					}									
				}
			}
			if((*juego).sombra.esta_viva){
				if(esta_dentro_rango((*juego).sombra.posicion,MAX_FILAS,MAX_COLUMNAS)&&(*juego).sombra.posicion.fila!=0&&(!hay_pared((*juego).niveles[nivel].paredes,(*juego).niveles[nivel].tope_paredes,(*juego).sombra.posicion,movimiento))){
					(*juego).sombra.posicion.fila -=PASITO ;
				}
			}	
		break;
		case ABAJO:
			if(esta_dentro_rango((*juego).personaje.posicion,MAX_FILAS,MAX_COLUMNAS)&&(*juego).personaje.posicion.fila!=MAX_FILAS-1){
				if((!hay_pared((*juego).niveles[nivel].paredes,(*juego).niveles[nivel].tope_paredes,(*juego).personaje.posicion,movimiento))){
					(*juego).personaje.posicion.fila +=PASITO;
				}
				else{
					pasos_a_dar=(paredes_y_escaleras((*juego).niveles[nivel].herramientas,(*juego).niveles[nivel].tope_herramientas,(*juego).niveles[nivel].paredes,(*juego).niveles[nivel].tope_paredes,movimiento,(*juego).personaje.posicion));
					for(int i=0;i<pasos_a_dar;i++){
						(*juego).personaje.posicion.fila +=PASITO;
						pos_escalera_eliminar=buscar_elemento((*juego).niveles[nivel].herramientas,(*juego).niveles[nivel].tope_herramientas,(*juego).personaje.posicion);
						eliminar_elemento((*juego).niveles[nivel].herramientas,(&(*juego).niveles[nivel].tope_herramientas),pos_escalera_eliminar);
					}					
				}
			}
			if((*juego).sombra.esta_viva){
				if(esta_dentro_rango((*juego).sombra.posicion,MAX_FILAS,MAX_COLUMNAS)&&(*juego).sombra.posicion.fila!=MAX_FILAS-1&&(!hay_pared((*juego).niveles[nivel].paredes,(*juego).niveles[nivel].tope_paredes,(*juego).sombra.posicion,movimiento))){
					(*juego).sombra.posicion.fila +=PASITO ;
				}
			}	
		break;
		case DERECHA:
			if(esta_dentro_rango((*juego).personaje.posicion,MAX_FILAS,MAX_COLUMNAS)&&(*juego).personaje.posicion.col!=MAX_COLUMNAS-1){
				if((!hay_pared((*juego).niveles[nivel].paredes,(*juego).niveles[nivel].tope_paredes,(*juego).personaje.posicion,movimiento))){	
					(*juego).personaje.posicion.col +=PASITO;
				}
				else{
					pasos_a_dar=(paredes_y_escaleras((*juego).niveles[nivel].herramientas,(*juego).niveles[nivel].tope_herramientas,(*juego).niveles[nivel].paredes,(*juego).niveles[nivel].tope_paredes,movimiento,(*juego).personaje.posicion));
					for(int i=0;i<pasos_a_dar;i++){
						(*juego).personaje.posicion.col +=PASITO;
						pos_escalera_eliminar=buscar_elemento((*juego).niveles[nivel].herramientas,(*juego).niveles[nivel].tope_herramientas,(*juego).personaje.posicion);
						eliminar_elemento((*juego).niveles[nivel].herramientas,(&(*juego).niveles[nivel].tope_herramientas),pos_escalera_eliminar);
					}									
				}
			}
			if((*juego).sombra.esta_viva){
				if(!(*juego).personaje.interruptor_apretado){
					if(esta_dentro_rango((*juego).sombra.posicion,MAX_FILAS,MAX_COLUMNAS)&&(*juego).sombra.posicion.col!=0&&(!hay_pared((*juego).niveles[nivel].paredes,(*juego).niveles[nivel].tope_paredes,(*juego).sombra.posicion,movimiento_espejo))){
						(*juego).sombra.posicion.col-=PASITO;
					}
				}
				else{
					if(esta_dentro_rango((*juego).sombra.posicion,MAX_FILAS,MAX_COLUMNAS)&&(*juego).sombra.posicion.col!=MAX_COLUMNAS-1&&(!hay_pared((*juego).niveles[nivel].paredes,(*juego).niveles[nivel].tope_paredes,(*juego).sombra.posicion,movimiento))){
						(*juego).sombra.posicion.col +=PASITO;
					}
				}
			}
		break;
		case PEDIR_VIDA:
			if(((*juego).personaje.vida<MAX_VIDAS)&&((*juego).personaje.puntos)>=PUNTOS_POR_VIDA){
				(*juego).personaje.vida+=1;
				(*juego).personaje.puntos-=200;
			}
		break;
	}
}
/* Procedimiento que llena una matriz de "CESPED"
*/
void vaciar_matriz(char terreno_vacio[MAX_FILAS][MAX_COLUMNAS]){
	for(int i=0;i<MAX_FILAS;i++){
		for(int j=0;j<MAX_COLUMNAS;j++){
			terreno_vacio[i][j]=CESPED;
		}
	}
}
/*
	Procedimiento que , segun las posiciones dadas del vector de paredes, carga las paredes en la matriz de terreno
 */
void cargar_paredes_en_matriz(char terreno[MAX_FILAS][MAX_COLUMNAS],coordenada_t paredes[MAX_PAREDES],int tope_paredes){
	for(int i=0;i<MAX_FILAS;i++){
		for(int j=0;j<MAX_COLUMNAS;j++){
			for(int h=0;h<tope_paredes;h++){
				if((i==paredes[h].fila)&&(j==paredes[h].col)){
					terreno[i][j]=PARED;
				}
			}
		}
	}
}
/*
 	Procedimiento que recibe un juego_t  y una vector de listas.Inicializa los 3 niveles con la configuracion solicitada por
 	el usuario en su respectiva posicion de niveles. 
 	Utiliza una matriz de prueba para que no se superpongan los obstaculos y herramientas
*/
void inicializar_niveles_config(juego_t* juego,lista_t lista_elementos[MAX_NIVELES]){
	char terreno_prueba[MAX_FILAS][MAX_COLUMNAS];
	int velas,pozos,interruptores,portales,escaleras,monedas,llaves,puerta;
	for(int o=POS_NIVEL_1;o<MAX_NIVELES;o++){

		(*juego).niveles[o].numero_nivel=(o+1);
		velas=lista_elementos[o].cantidad_velas;
		pozos=lista_elementos[o].cantidad_pozos;
		interruptores=lista_elementos[o].cantidad_interruptores;
		portales=lista_elementos[o].cantidad_portales;
		escaleras=lista_elementos[o].cantidad_escaleras;
		monedas=lista_elementos[o].cantidad_monedas;
		llaves=lista_elementos[o].cantidad_llaves;
		puerta=lista_elementos[o].cantidad_puertas;

		vaciar_matriz(terreno_prueba);
		(*juego).niveles[o].tope_herramientas=0;
		(*juego).niveles[o].tope_obstaculos=0;
		obtener_mapa((juego)->niveles[o].paredes,&(juego->niveles[o].tope_paredes));
		cargar_paredes_en_matriz(terreno_prueba,(juego)->niveles[o].paredes,juego->niveles[o].tope_paredes);

		for(int i=0;i<(velas+pozos+interruptores+portales);i++){
			if(i<velas){
				(*juego).niveles[o].obstaculos[i].tipo=VELA;
				(*juego).niveles[o].obstaculos[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				while(terreno_prueba[(*juego).niveles[o].obstaculos[i].coordenada.fila][(*juego).niveles[o].obstaculos[i].coordenada.col]!=CESPED){
					(*juego).niveles[o].obstaculos[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				}
				terreno_prueba[(*juego).niveles[o].obstaculos[i].coordenada.fila][(*juego).niveles[o].obstaculos[i].coordenada.col]=VELA;
				(*juego).niveles[o].tope_obstaculos++;
			}
			else if(i<(velas+pozos)){
				(*juego).niveles[o].obstaculos[i].tipo=POZO;
				(*juego).niveles[o].obstaculos[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				while(terreno_prueba[(*juego).niveles[o].obstaculos[i].coordenada.fila][(*juego).niveles[o].obstaculos[i].coordenada.col]!=CESPED){
					(*juego).niveles[o].obstaculos[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				}
				terreno_prueba[(*juego).niveles[o].obstaculos[i].coordenada.fila][(*juego).niveles[o].obstaculos[i].coordenada.col]=POZO;
				(*juego).niveles[o].tope_obstaculos++;
			}
			else if(i<(velas+pozos+interruptores)){
				(*juego).niveles[o].obstaculos[i].tipo=INTERRUPTOR;
				(*juego).niveles[o].obstaculos[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				while(terreno_prueba[(*juego).niveles[o].obstaculos[i].coordenada.fila][(*juego).niveles[o].obstaculos[i].coordenada.col]!=CESPED){
					(*juego).niveles[o].obstaculos[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				}
				terreno_prueba[(*juego).niveles[o].obstaculos[i].coordenada.fila][(*juego).niveles[o].obstaculos[i].coordenada.col]=INTERRUPTOR;
				(*juego).niveles[o].tope_obstaculos++;
			}
			else if(i<(velas+pozos+interruptores+portales)){
				(*juego).niveles[o].obstaculos[i].tipo=PORTAL;
				(*juego).niveles[o].obstaculos[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				while(terreno_prueba[(*juego).niveles[o].obstaculos[i].coordenada.fila][(*juego).niveles[o].obstaculos[i].coordenada.col]!=CESPED){
					(*juego).niveles[o].obstaculos[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				}
				terreno_prueba[(*juego).niveles[o].obstaculos[i].coordenada.fila][(*juego).niveles[o].obstaculos[i].coordenada.col]=PORTAL;
				(*juego).niveles[o].tope_obstaculos++;
			}
		}
		for(int i=0;i<(escaleras+monedas+puerta+llaves);i++){
			if(i<escaleras){
				(*juego).niveles[o].herramientas[i].tipo=ESCALERA;
				(*juego).niveles[o].herramientas[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				while(terreno_prueba[(*juego).niveles[o].herramientas[i].coordenada.fila][(*juego).niveles[o].herramientas[i].coordenada.col]!=PARED){
					(*juego).niveles[o].herramientas[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				}
				(*juego).niveles[o].tope_herramientas++;
				terreno_prueba[(*juego).niveles[o].herramientas[i].coordenada.fila][(*juego).niveles[o].herramientas[i].coordenada.col]=ESCALERA;
			}
			else if(i<(escaleras+monedas)){
				(*juego).niveles[o].herramientas[i].tipo=MONEDA;
				(*juego).niveles[o].herramientas[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				while(terreno_prueba[(*juego).niveles[o].herramientas[i].coordenada.fila][(*juego).niveles[o].herramientas[i].coordenada.col]!=CESPED){
					(*juego).niveles[o].herramientas[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				}
				terreno_prueba[(*juego).niveles[o].herramientas[i].coordenada.fila][(*juego).niveles[o].herramientas[i].coordenada.col]=MONEDA;
				(*juego).niveles[o].tope_herramientas++;
			}
			else if(i<escaleras+monedas+puerta){
				(*juego).niveles[o].herramientas[i].tipo=PUERTA;
				(*juego).niveles[o].herramientas[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				while(terreno_prueba[(*juego).niveles[o].herramientas[i].coordenada.fila][(*juego).niveles[o].herramientas[i].coordenada.col]!=CESPED){
					(*juego).niveles[o].herramientas[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				}
				terreno_prueba[(*juego).niveles[o].herramientas[i].coordenada.fila][(*juego).niveles[o].herramientas[i].coordenada.col]=PUERTA_POR_NIVEL;
				(*juego).niveles[o].tope_herramientas++;
			}
			else if(i<(escaleras+monedas+puerta+llaves)){
				(*juego).niveles[o].herramientas[i].tipo=LLAVE;
				(*juego).niveles[o].herramientas[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				while(terreno_prueba[(*juego).niveles[o].herramientas[i].coordenada.fila][(*juego).niveles[o].herramientas[i].coordenada.col]!=CESPED){
					(*juego).niveles[o].herramientas[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				}
				terreno_prueba[(*juego).niveles[o].herramientas[i].coordenada.fila][(*juego).niveles[o].herramientas[i].coordenada.col]=LLAVE;
				(*juego).niveles[o].tope_herramientas++;
			}	
		}
	}
}
/*
*	Procedimiento que carga las variables de obstaculos y herramientas para inicializar el nivel correspondiente
*/
void cargar_cantidades(int o,int* nivel,int* velas,int* pozos,int* interruptores,int* portales,int* escaleras,int* monedas,int* llaves,int* puerta){
	if(o==POS_NIVEL_1){
		(*nivel)=NIVEL_1;
		(*velas)=VELAS_NIVEL_1;
		(*pozos)=POZOS_NIVEL_1;
		(*interruptores)=INTERRUPTORES_NIVEL_1;
		(*portales)=PORTALES_NIVEL_1;
		(*escaleras)=ESCALERAS_NIVEL_1;
		(*monedas)=MONEDAS_NIVEL_1;
		(*llaves)=LLAVES_NIVEL_1;
		(*puerta)=PUERTA_POR_NIVEL;
	}	
	else if(o==POS_NIVEL_2){
		(*nivel)=NIVEL_2;
		(*velas)=VELAS_NIVEL_2;
		(*pozos)=POZOS_NIVEL_2;
		(*interruptores)=INTERRUPTORES_NIVEL_2;
		(*portales)=PORTALES_NIVEL_2;
		(*escaleras)=ESCALERAS_NIVEL_2;
		(*monedas)=MONEDAS_NIVEL_2;
		(*llaves)=LLAVES_NIVEL_2;
		(*puerta)=PUERTA_POR_NIVEL;
	}
	else{
		(*nivel)=NIVEL_3;
		(*velas)=VELAS_NIVEL_3;
		(*pozos)=POZOS_NIVEL_3;
		(*interruptores)=INTERRUPTORES_NIVEL_3;
		(*portales)=PORTALES_NIVEL_3;
		(*escaleras)=ESCALERAS_NIVEL_3;
		(*monedas)=MONEDAS_NIVEL_3;
		(*llaves)=LLAVES_NIVEL_3;
		(*puerta)=PUERTA_POR_NIVEL;
	}
}
/*
	 Procedimiento que recibe un juego_t e inicializa los 3 niveles en su respectiva posicion de niveles.
 	Utiliza una matriz de prueba para que no se superpongan los obstaculos y herramientas
*/
void inicializar_niveles(juego_t* juego){
	char terreno_prueba[MAX_FILAS][MAX_COLUMNAS];
	int velas,pozos,interruptores,portales,escaleras,monedas,llaves,puerta;
	for(int o=POS_NIVEL_1;o<MAX_NIVELES;o++){
		cargar_cantidades(o,&((*juego).niveles[o].numero_nivel),&velas,&pozos,&interruptores,&portales,&escaleras,&monedas,&llaves,&puerta);
		vaciar_matriz(terreno_prueba);
		(*juego).niveles[o].tope_herramientas=0;
		(*juego).niveles[o].tope_obstaculos=0;
		obtener_mapa((juego)->niveles[o].paredes,&(juego->niveles[o].tope_paredes));
		cargar_paredes_en_matriz(terreno_prueba,(juego)->niveles[o].paredes,juego->niveles[o].tope_paredes);
		
		for(int i=0;i<(velas+pozos+interruptores+portales);i++){
			if(i<velas){
				(*juego).niveles[o].obstaculos[i].tipo=VELA;
				(*juego).niveles[o].obstaculos[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				while(terreno_prueba[(*juego).niveles[o].obstaculos[i].coordenada.fila][(*juego).niveles[o].obstaculos[i].coordenada.col]!=CESPED){
					(*juego).niveles[o].obstaculos[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				}
				terreno_prueba[(*juego).niveles[o].obstaculos[i].coordenada.fila][(*juego).niveles[o].obstaculos[i].coordenada.col]=VELA;
				(*juego).niveles[o].tope_obstaculos++;
			}
			else if(i<(velas+pozos)){
				(*juego).niveles[o].obstaculos[i].tipo=POZO;
				(*juego).niveles[o].obstaculos[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				while(terreno_prueba[(*juego).niveles[o].obstaculos[i].coordenada.fila][(*juego).niveles[o].obstaculos[i].coordenada.col]!=CESPED){
					(*juego).niveles[o].obstaculos[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				}
				terreno_prueba[(*juego).niveles[o].obstaculos[i].coordenada.fila][(*juego).niveles[o].obstaculos[i].coordenada.col]=POZO;
				(*juego).niveles[o].tope_obstaculos++;
			}
			else if(i<(velas+pozos+interruptores)){
				(*juego).niveles[o].obstaculos[i].tipo=INTERRUPTOR;
				(*juego).niveles[o].obstaculos[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				while(terreno_prueba[(*juego).niveles[o].obstaculos[i].coordenada.fila][(*juego).niveles[o].obstaculos[i].coordenada.col]!=CESPED){
					(*juego).niveles[o].obstaculos[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				}
				terreno_prueba[(*juego).niveles[o].obstaculos[i].coordenada.fila][(*juego).niveles[o].obstaculos[i].coordenada.col]=INTERRUPTOR;
				(*juego).niveles[o].tope_obstaculos++;
			}
			else if(i<(velas+pozos+interruptores+portales)){
				(*juego).niveles[o].obstaculos[i].tipo=PORTAL;
				(*juego).niveles[o].obstaculos[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				while(terreno_prueba[(*juego).niveles[o].obstaculos[i].coordenada.fila][(*juego).niveles[o].obstaculos[i].coordenada.col]!=CESPED){
					(*juego).niveles[o].obstaculos[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				}
				terreno_prueba[(*juego).niveles[o].obstaculos[i].coordenada.fila][(*juego).niveles[o].obstaculos[i].coordenada.col]=PORTAL;
				(*juego).niveles[o].tope_obstaculos++;
			}
		}

		for(int i=0;i<(escaleras+monedas+puerta+llaves);i++){
			if(i<escaleras){
				(*juego).niveles[o].herramientas[i].tipo=ESCALERA;
				(*juego).niveles[o].herramientas[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				while(terreno_prueba[(*juego).niveles[o].herramientas[i].coordenada.fila][(*juego).niveles[o].herramientas[i].coordenada.col]!=PARED){
					(*juego).niveles[o].herramientas[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				}
				(*juego).niveles[o].tope_herramientas++;
				terreno_prueba[(*juego).niveles[o].herramientas[i].coordenada.fila][(*juego).niveles[o].herramientas[i].coordenada.col]=ESCALERA;
			}
			else if(i<(escaleras+monedas)){
				(*juego).niveles[o].herramientas[i].tipo=MONEDA;
				(*juego).niveles[o].herramientas[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				while(terreno_prueba[(*juego).niveles[o].herramientas[i].coordenada.fila][(*juego).niveles[o].herramientas[i].coordenada.col]!=CESPED){
					(*juego).niveles[o].herramientas[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				}
				terreno_prueba[(*juego).niveles[o].herramientas[i].coordenada.fila][(*juego).niveles[o].herramientas[i].coordenada.col]=MONEDA;
				(*juego).niveles[o].tope_herramientas++;
			}
			else if(i<escaleras+monedas+puerta){
				(*juego).niveles[o].herramientas[i].tipo=PUERTA;
				(*juego).niveles[o].herramientas[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				while(terreno_prueba[(*juego).niveles[o].herramientas[i].coordenada.fila][(*juego).niveles[o].herramientas[i].coordenada.col]!=CESPED){
					(*juego).niveles[o].herramientas[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				}
				terreno_prueba[(*juego).niveles[o].herramientas[i].coordenada.fila][(*juego).niveles[o].herramientas[i].coordenada.col]=PUERTA_POR_NIVEL;
				(*juego).niveles[o].tope_herramientas++;
			}
			else if(i<(escaleras+monedas+puerta+llaves)){
				(*juego).niveles[o].herramientas[i].tipo=LLAVE;
				(*juego).niveles[o].herramientas[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				while(terreno_prueba[(*juego).niveles[o].herramientas[i].coordenada.fila][(*juego).niveles[o].herramientas[i].coordenada.col]!=CESPED){
					(*juego).niveles[o].herramientas[i].coordenada=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
				}
				terreno_prueba[(*juego).niveles[o].herramientas[i].coordenada.fila][(*juego).niveles[o].herramientas[i].coordenada.col]=LLAVE;
				(*juego).niveles[o].tope_herramientas++;
			}	
		}
	}
}
/*
	Procedimiento que cargar la matriz terreno a imprimir con todos los obstaculos, herramientas, paredes y los personajes
*/
void cargar_matriz(char terreno[MAX_FILAS][MAX_COLUMNAS],juego_t juego){
	int nivel=(juego.nivel_actual-1);//variable que uso para resumir , cuando se usa el vector de niveles
	for(int i=0;i<juego.niveles[nivel].tope_paredes;i++){
		terreno[juego.niveles[nivel].paredes[i].fila][juego.niveles[nivel].paredes[i].col]=PARED;
	}
	for(int i=0;i<juego.niveles[nivel].tope_obstaculos;i++){
		terreno[juego.niveles[nivel].obstaculos[i].coordenada.fila][juego.niveles[nivel].obstaculos[i].coordenada.col]= juego.niveles[nivel].obstaculos[i].tipo;
	}
	crear_matriz_de_efecto_velas(terreno,(juego.niveles[nivel].obstaculos),(juego.niveles[nivel].tope_obstaculos));
	for(int i=0;i<juego.niveles[nivel].tope_herramientas;i++){
		terreno[juego.niveles[nivel].herramientas[i].coordenada.fila][juego.niveles[nivel].herramientas[i].coordenada.col]= juego.niveles[nivel].herramientas[i].tipo;
	}
	terreno[juego.personaje.posicion.fila][juego.personaje.posicion.col]=MAC;
	terreno[juego.sombra.posicion.fila][juego.sombra.posicion.col]=BLOO;
}
/*	Procedimiento auxiliar de imprimir_terreno, que imprime por pantalla el numero de columnas para ser mas legible
*/
void imprimir_contador_de_columnas(){
	int contador_columnas;
	for(contador_columnas=0;contador_columnas<MAX_COLUMNAS;contador_columnas++){
		if(contador_columnas<10){
			printf(" %i ",contador_columnas );
		}
		else{
			printf("%i ",contador_columnas);
		}
	}
	printf("\n");	
}
/*	Procedimiento auxiliar de imprimir_terreno, que imprime por pantalla el numero de filas para ser mas legible
*/
void imprimir_contador_de_filas(int* contador_filas){
		printf("%i",(*contador_filas) );
		printf("\n");
		(*contador_filas)++;
}
/*
* Procedimiento auxiliar de imprimir_terreno , le da color a algunos caracteres para una lectura mas facil
*/
void imprimir_con_color(char caracter){
	if(caracter==PARED){
		printf(ANSI_COLOR_RED " %c " ANSI_COLOR_RESET ,caracter);
	}
	else if(caracter==MAC){
		printf(ANSI_COLOR_BLUE " %c " ANSI_COLOR_RESET ,caracter);
	}
	else if(caracter==CESPED){
		printf(ANSI_COLOR_GREEN " %c "ANSI_COLOR_RESET,caracter);
	}
	else if(caracter==VELA){
		printf(ANSI_COLOR_YELLOW  " %c "ANSI_COLOR_RESET,caracter);
	}
	else if(caracter==AREA_EFECTO_VELA){
		printf(ANSI_COLOR_YELLOW  " %c "ANSI_COLOR_RESET,caracter);
	}
	else if(caracter==BLOO){
		printf(ANSI_COLOR_MAGENTA " %c "ANSI_COLOR_RESET,caracter);
	}
	else if(caracter==PUERTA){
		printf(ANSI_COLOR_CYAN " %c "ANSI_COLOR_RESET,caracter);
	}
	else{
		printf(" %c ",caracter);
	}
}
/*
*    Procedimiento que recibe el juego e imprime 
* toda su información por pantalla.
*/
void imprimir_terreno(juego_t juego){
	char terreno[MAX_FILAS][MAX_COLUMNAS];
	int contador_filas=0; 
	vaciar_matriz(terreno);
	cargar_matriz(terreno,juego);
	imprimir_contador_de_columnas();
	for (int i = 0; i < MAX_FILAS;i++){
		for(int j=0;j<MAX_COLUMNAS;j++){
			if(coordenadas_iguales(juego.personaje.posicion,juego.sombra.posicion)&&(i==juego.personaje.posicion.fila&&j==juego.personaje.posicion.col)){
				printf(" %c ",AMBOS);	
			}
			else{
				imprimir_con_color(terreno[i][j]);
			}
		}
		imprimir_contador_de_filas(&contador_filas);
	}
}
void inicializar_juego(juego_t* juego,lista_t lista_elementos[MAX_NIVELES],bool hay_config){
	(*juego).nivel_actual=NIVEL_1;
	cargar_mapas();
	if(hay_config){
		inicializar_niveles_config(juego,lista_elementos);
	}
	else{
		inicializar_niveles(juego);
	}
	coordenada_t arranque_personaje=coordenada_aleatoria(MAX_FILAS, MAX_COLUMNAS);
	coordenada_t arranque_sombra;
	arranque_sombra.fila=arranque_personaje.fila;
	arranque_sombra.col=((MAX_COLUMNAS-arranque_personaje.col)-1);

	while(!pueden_arrancar((*juego).niveles[POS_NIVEL_1].paredes,(*juego).niveles[POS_NIVEL_1].tope_paredes,arranque_personaje,arranque_sombra)
		||(arranque_personaje.fila>MAX_FILAS-1)||(arranque_personaje.col>MAX_COLUMNAS-1)){
		arranque_personaje=coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
		arranque_sombra.fila=arranque_personaje.fila;
		arranque_sombra.col=((MAX_COLUMNAS-arranque_personaje.col)-1);
	}
	inicializar_personaje(&(juego)->personaje,arranque_personaje);
	inicializar_sombra(&(juego)->sombra,arranque_sombra);
}
void realizar_jugada(juego_t* juego){
	char movimiento;
	pedir_movimiento(&movimiento);
	mover_personajes_o_saltar_escaleras(juego,movimiento);
	chequear_obstaculos(juego);
}
int estado_juego(juego_t juego){
	if((juego.nivel_actual==NIVEL_3)&&(juego.personaje.vida>0)&&(juego.sombra.esta_viva)&&(estado_nivel(juego)==GANADO)){
		return GANADO;
	}
	else{
		if(juego.personaje.vida==0){
			return PERDIDO;
		}
		else{
			return JUGANDO;
		}
	}
} 
int estado_nivel(juego_t juego){
	int nivel=(juego.nivel_actual)-1;
	if(juego.nivel_actual==NIVEL_1){
		if(estan_distancia_puerta(juego.niveles[nivel].herramientas,juego.niveles[nivel].tope_herramientas,juego.personaje.posicion,juego.sombra.posicion)){
			return GANADO;
		}
		else{
			return JUGANDO;
		}
	}
	else{
		if(estan_distancia_puerta(juego.niveles[nivel].herramientas,juego.niveles[nivel].tope_herramientas,juego.personaje.posicion,juego.sombra.posicion)&&(juego.personaje.tiene_llave)&&(juego.sombra.esta_viva)){
			return GANADO;
		}
		else{
			return JUGANDO;
		}
	}
}