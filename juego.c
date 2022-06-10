#include "la_sombra_de_mac.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define JUEGO_TERMINADO 'Z'
#define GANADO 1
#define PERDIDO -1
#define JUGANDO 0
#define NIVEL_1 1
#define NIVEL_2 2
#define NIVEL_3 3
#define CORAZON "\u2764"
/*
 Procedimiento que limpia la pantalla y luego imprime  un cartel dependiendo de si estas jugando, en que nivel te encontras,
 o si el juego termino , imprime el mensaje de GANASTE o PERDISTE
*/
void imprimir_estado_de_juego(int nivel_actual,int estado_juego){
	system("clear");
	if(estado_juego==GANADO){
		printf("\
..######......###....##....##....###.....######..########.########\n\
.##....##....##.##...###...##...##.##...##....##....##....##......\n\
.##.........##...##..####..##..##...##..##..........##....##......\n\
.##...####.##.....##.##.##.##.##.....##..######.....##....######..\n\
.##....##..#########.##..####.#########.......##....##....##......\n\
.##....##..##.....##.##...###.##.....##.##....##....##....##......\n\
..######...##.....##.##....##.##.....##..######.....##....########\n");

	}
	else if(estado_juego==PERDIDO){
		printf("\
.########..########.########..########..####..######..########.########\n\
.##.....##.##.......##.....##.##.....##..##..##....##....##....##......\n\
.##.....##.##.......##.....##.##.....##..##..##..........##....##......\n\
.########..######...########..##.....##..##...######.....##....######..\n\
.##........##.......##...##...##.....##..##........##....##....##......\n\
.##........##.......##....##..##.....##..##..##....##....##....##......\n\
.##........########.##.....##.########..####..######.....##....########\n");
	}
	else{
		if(nivel_actual==NIVEL_1){
			printf("\
.##....##.####.##.....##.########.##.............##..\n\
.###...##..##..##.....##.##.......##...........####..\n\
.####..##..##..##.....##.##.......##.............##..\n\
.##.##.##..##..##.....##.######...##.............##..\n\
.##..####..##...##...##..##.......##.............##..\n\
.##...###..##....##.##...##.......##.............##..\n\
.##....##.####....###....########.########.....######\n");
		}
		else if(nivel_actual==NIVEL_2){
			printf("\
.##....##.####.##.....##.########.##...........#######.\n\
.###...##..##..##.....##.##.......##..........##.....##\n\
.####..##..##..##.....##.##.......##.................##\n\
.##.##.##..##..##.....##.######...##...........#######.\n\
.##..####..##...##...##..##.......##..........##.......\n\
.##...###..##....##.##...##.......##..........##.......\n\
.##....##.####....###....########.########....#########\n");

		}
		else{
			printf("\
.##....##.####.##.....##.########.##...........#######.\n\
.###...##..##..##.....##.##.......##..........##.....##\n\
.####..##..##..##.....##.##.......##.................##\n\
.##.##.##..##..##.....##.######...##...........#######.\n\
.##..####..##...##...##..##.......##.................##\n\
.##...###..##....##.##...##.......##..........##.....##\n\
.##....##.####....###....########.########.....#######.\n");
		}
	}
}
/*
 Procedimiento que actualiza,los datos del juego y se encarga de limpiar la pantalla
*/
void actualizar_datos_juego(personaje_t personaje,sombra_t sombra){
	printf("VIDAS DE MAC: ");
	for (int i = 0; i < personaje.vida;i++){
		printf("%s",CORAZON );
	}
	printf("\t");
	printf("PUNTOS DE MAC: %i \t",personaje.puntos);
	if(personaje.tiene_llave==true){
		printf("MAC TIENE LA LLAVE\t");
	}
	else{
		printf("MAC NO TIENE LA LLAVE\t");
	}
	if(sombra.esta_viva){
		printf("BLOO ESTA VIVO\n");
	}
	else{
		printf("BLOO NO ESTA VIVO\n");
	}
}
int main(){
	juego_t juego;
	srand((unsigned) time(NULL));
	inicializar_juego(&juego);
	while(estado_juego(juego)==JUGANDO){
		imprimir_estado_de_juego(juego.nivel_actual,estado_juego(juego));
		imprimir_terreno(juego);
		actualizar_datos_juego(juego.personaje,juego.sombra);
		realizar_jugada(&juego);
		if((estado_nivel(juego))==GANADO){
			if((juego).nivel_actual!=NIVEL_3){
				(juego).nivel_actual+=1;
			}
		}
	}
	imprimir_estado_de_juego(juego.nivel_actual,estado_juego(juego));
	return 0;
}