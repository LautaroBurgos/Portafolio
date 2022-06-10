#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define LEER_PARTIDAS "%[^;];%i;%i;%i;%[^\n]\n"
#define ESCRIBIR_PARTIDAS "%s;%i;%i;%i;%s\n"
#define AGREGAR_PARTIDA "agregar_partida"
#define ELIMINAR_PARTIDA "eliminar_partida"
#define ORDENAR_PARTIDAS "ordenar_partidas"
#define MAX_NOMBRE 100
#define MAX_PARTIDAS 30
#define NIVEL_1 1
#define NIVEL_2 2
#define NIVEL_3 3
#define SI "Si"
#define NO "No"
#define MAX_CARACTERES 30
#define CARACTER_ESCALERA 'E'
#define CARACTER_LLAVE 'L'
#define CARACTER_MONEDA 'C'
#define CARACTER_POZO 'W'
#define CARACTER_INTERRUPTOR 'O'
#define CARACTER_PORTAL 'P'
#define CARACTER_VELA 'V'
#define CARACTER_PUERTA 'D'

#define ESCALERA "ESCALERAS"
#define LLAVE "LLAVES"
#define MONEDA "MONEDAS"
#define POZO "POZOS"
#define INTERRUPTOR "INTERRUPTORES"
#define PORTAL "PORTALES"
#define VELA "VELAS"
#define PUERTA "PUERTAS"

#define ETIQUETA_NIVEL_1 "N1"
#define ETIQUETA_NIVEL_2 "N2"
#define ETIQUETA_NIVEL_3 "N3"
#define POS_NIVEL_1 0
#define POS_NIVEL_2 1
#define POS_NIVEL_3 2
#define LECTURA_CONFIG "%[^_]_%[^=]=%i\n"
typedef struct partida{
	char jugador[MAX_NOMBRE];
	int nivel_llegado;
	int puntos;
	int vidas_restantes;
	bool gano;
}partida_t;
/*
	Procedimiento que lee las partidas del archivo seleccionado y las guarda en un vector de partidas para que luego
	sea usado en las demas funciones(agregar,eliminar u ordenar partidas).
	PRE: El tope recibido debe estar inicializado en 0.
	POST:Llena un vector de partidas(En el mismo orden que estaban en el archivo) y su tope actualizado.
*/
void llenar_struct_de_partidas(FILE* lista_partidas,partida_t partidas[MAX_PARTIDAS],int* tope_partidas);
/*
	Funcion utilizada para validar el dato ingresado de los niveles,devuelve true si es un valor valido(1,2,o 3) o 
	false en caso contrario.
*/
bool es_nivel_valido(int nivel);
/*	
	Funcion usada para validar el dato ingresado de las vidas,devuelve true si es un valor valido (entre 0 y 3 inclusive)
	o false en caso contrario.
*/
bool es_vida_valida(int vidas);
/*	
	Funcion que valida el formato del "Si" o "No" que se utiliza para representar en el archivo si el  usuario
	gano o no el juego. Devuelve true si cumple y devuelve false en caso contrario.
*/
bool es_formato_valido(char formato[10]);
/*
	Procedimiento que solicita y almacena los datos de la partida que se va a ingresar
	PRE:
	-El nombre del jugador a ingresar debe ser SI O SI SIN ESPACIOS,se recomienda usar "_".
	-Los valores, vidas_restantes, nivel_llegado, y el string que guarda Si gano o No , van a ser validados, hasta que
	esten dentro de los valores o formato establecido.

	POST: Al finalizar el procedimiento , la partida_a_agregar queda con valores validos y lista para que sea
	escrita en el archivo .
*/
void ingresar_datos_de_partida(partida_t* partida_a_agregar);
/*
	Procedimiento que agrega una partida ingresada por consola, al archivo seleccionado respetando el orden preestablecido
	PRE: El archivo a recibir debe estar a abierto para escritura
	POST: El archivo seleccionado queda editado con la partida agregada por el usuario, en el lugar que le corresponda
	por orden alfabetico.
*/
void agregar_partida(FILE* aux,partida_t partidas[MAX_PARTIDAS],int tope_partidas);
/*
	Procedimiento que elimina  una partida, de un archivo de partidas seleccionado,la busca a traves del nombre ingresado.
	PRE: El archivo que recibe debe estar en formato de escritura.
	POST:El archivo de partidas queda editado sin la partida que se mando a eliminar, en el mismo orden preestablecido,
	en caso de no haber encontrado una partida con el nombre ingresado, no hace nada.
*/
void eliminar_partida(FILE* aux,partida_t partidas[MAX_PARTIDAS],int tope_partidas,char jugador_a_eliminar[MAX_NOMBRE]);
/*
	Procedimiento auxiliar de  ordenar_partidas, ordena el vector de partidas que se le envia de forma alfabetica.
*/
void ordenar_aux(partida_t partidas[MAX_PARTIDAS],int tope_partidas);
/*
	Procedimiento que recibe un vector de partidas,lo ordena , y luego lo imprime ordenado en el archivo que se 
	le pasa por parametro.
	PRE: El archivo de partidas tiene que estar en modo de escritura. 
		-El vector de partidas puede estar desordenado.
	POST: El archivo de partidas queda ordenado alfabeticamente.
*/
void ordenar_partidas(FILE* aux,partida_t partidas[MAX_PARTIDAS],int tope_partidas);