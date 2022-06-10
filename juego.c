#include "la_sombra_de_mac.h"
#include "mansion_foster.h"
#include<stdlib.h>
#include<time.h>
#define GANADO 1
#define PERDIDO -1
#define JUGANDO 0
#define NIVEL_1 1
#define NIVEL_2 2
#define NIVEL_3 3
#define CORAZON "\u2764"
#define CONFIGURAR_JUEGO "config_juego"
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
		printf("MAC TIENE LA/S LLAVE/S\t");
	}
	else{
		printf("MAC NO TIENE LA/S LLAVE/S\t\t");
	}
	if(sombra.esta_viva){
		printf("BLOO ESTA VIVO\n");
	}
	else{
		printf("BLOO NO ESTA VIVO\n");
	}
}
/*
	Procedimiento que lee un archivo de partidas y vierte su contenido en un struct de tipo lista_t.
	Los elementos configurables son: POZOS,VELAS,PORTALES, ESCALERAS,LLAVES,INTERRUPTORES,MONEDAS y PUERTAS.
	PRE: El archivo debe cumplir con el formato pre establecido
	N<NIVEL>_<ELEMENTO A CONFIGURAR>=<CANTIDAD DE ELEMENTO>
	->La cantidad de llaves en el NIVEL 1 siempre debe ser 0.
	->El archivo puede tener los valores desordenados.
*/
void leer_datos_config(lista_t lista[MAX_NIVELES],FILE* configuracion){
	char etiqueta[MAX_CARACTERES];
	char elemento[MAX_CARACTERES];
	int cantidad_de_elemento;
	int nivel_a_configurar;
	while(!feof(configuracion)){
		fscanf(configuracion,LECTURA_CONFIG,etiqueta,elemento,&cantidad_de_elemento);
		if(strcmp(etiqueta,ETIQUETA_NIVEL_1)==0){
			nivel_a_configurar=POS_NIVEL_1;
		}
		else if(strcmp(etiqueta,ETIQUETA_NIVEL_2)==0){
			nivel_a_configurar=POS_NIVEL_2;
		}
		else{
			nivel_a_configurar=POS_NIVEL_3;
		}
		if(strcmp(elemento,POZO)==0){
			lista[nivel_a_configurar].cantidad_pozos=cantidad_de_elemento;
		}
		else if(strcmp(elemento,ESCALERA)==0){
			lista[nivel_a_configurar].cantidad_escaleras=cantidad_de_elemento;
		}
		else if(strcmp(elemento,VELA)==0){
			lista[nivel_a_configurar].cantidad_velas=cantidad_de_elemento;
		}
		else if(strcmp(elemento,INTERRUPTOR)==0){
			lista[nivel_a_configurar].cantidad_interruptores=cantidad_de_elemento;
		}
		else if(strcmp(elemento,PORTAL)==0){
			lista[nivel_a_configurar].cantidad_portales=cantidad_de_elemento;
		}
		else if(strcmp(elemento,MONEDA)==0){
			lista[nivel_a_configurar].cantidad_monedas=cantidad_de_elemento;
		}
		else if(strcmp(elemento,LLAVE)==0){
			lista[nivel_a_configurar].cantidad_llaves=cantidad_de_elemento;
		}
		else if(strcmp(elemento,PUERTA)==0){
			lista[nivel_a_configurar].cantidad_puertas=cantidad_de_elemento;
		}
	}
}
/*
	Procedimiento que imprime por pantalla si el juego esta configurado por el usuario o no,
	en caso de estarlo,imprime las diferentes cantidades de obstaculos y herramientas que hay en el nivel actual , y que 
	fue configurado mediante el config.txt.
*/
void imprimir_configuracion(elemento_t obstaculos[MAX_ELEMENTOS],int tope_obstaculos,elemento_t herramientas[MAX_ELEMENTOS],int tope_herramientas,bool hay_config){
	if(hay_config){	
		int velas=0;
		int pozos=0;
		int llaves=0;
		int escaleras=0;
		int portales=0;
		int monedas=0;
		int interruptores=0;
		int puertas=0;
		for(int i=0;i<tope_obstaculos;i++){
			if(obstaculos[i].tipo==CARACTER_VELA){
				velas++;
			}
			else if(obstaculos[i].tipo==CARACTER_POZO){
				pozos++;
			}
			else if(obstaculos[i].tipo==CARACTER_INTERRUPTOR){
				interruptores++;
			}
			else if(obstaculos[i].tipo==CARACTER_PORTAL){
				portales++;
			}
		}
		for(int i=0;i<tope_herramientas;i++){
			if(herramientas[i].tipo==CARACTER_MONEDA){
				monedas++;
			}
			else if(herramientas[i].tipo==CARACTER_LLAVE){
				llaves++;
			}
			else if(herramientas[i].tipo==CARACTER_ESCALERA){
				escaleras++;
			}
			else{
				puertas++;
			} 
		}
		printf("JUEGO CONFIGURADO POR EL USUARIO\n");
		printf("VELAS:%i \tPOZOS:%i \tPORTALES:%i \tINTERRUPTORES:%i\n",velas,pozos,portales,interruptores);
		printf("MONEDAS:%i \tLLAVES:%i \tESCALERAS:%i \tPUERTAS:%i\n",monedas,llaves,escaleras,puertas);
	}
	else{
		printf("JUEGO CONFIGURADO POR DEFECTO\n");
	}
}
int main(int argc,char* argv[]){
	srand((unsigned) time(NULL));
	if(argc<3){
		juego_t juego;
		lista_t lista_elementos[MAX_NIVELES];
		bool hay_config=false;
		FILE* configuracion = fopen("config.txt","r");
		if(!configuracion){
			fclose(configuracion);
			if(!configuracion){
				perror("No se pudo abrir el archivo");
				return -1;
			}
		}
		if(argc==2){
			if(strcmp(argv[1],CONFIGURAR_JUEGO)==0){
				hay_config=true;
				leer_datos_config(lista_elementos,configuracion);
			}
			else{
				perror("Comando invalido");
				return -1;
			}
		}

		inicializar_juego(&juego,lista_elementos,hay_config);
		while(estado_juego(juego)==JUGANDO){
			imprimir_estado_de_juego(juego.nivel_actual,estado_juego(juego));
			imprimir_terreno(juego);
			actualizar_datos_juego(juego.personaje,juego.sombra);
			imprimir_configuracion(juego.niveles[juego.nivel_actual-1].obstaculos,juego.niveles[juego.nivel_actual-1].tope_obstaculos,juego.niveles[juego.nivel_actual-1].herramientas,juego.niveles[juego.nivel_actual-1].tope_herramientas,hay_config);
			realizar_jugada(&juego);
			if((estado_nivel(juego))==GANADO){
				if((juego).nivel_actual!=NIVEL_3){
					(juego).nivel_actual+=1;
				}
			}
		}
		imprimir_estado_de_juego(juego.nivel_actual,estado_juego(juego));
	}
	else if(argc>=3){
		partida_t partidas[MAX_PARTIDAS];
		int tope_partidas=0;
		char archivo_a_editar[MAX_NOMBRE];
		char jugador_a_eliminar[MAX_NOMBRE];
		bool agregar=false;
		bool eliminar=false;
		bool ordenar=false;
		strcpy(archivo_a_editar,argv[2]);
		
		if(strcmp(argv[1],AGREGAR_PARTIDA)==0){
			if(argc==3){
				agregar=true;
			}
			else{
				perror("Comando invalido");
				return -1;
			}
		}
		else if(strcmp(argv[1],ORDENAR_PARTIDAS)==0){
			if(argc==3){
				ordenar=true;
			}
			else{
				perror("Comando invalido");
				return -1;
			}
		}
		else if(strcmp(argv[1],ELIMINAR_PARTIDA)==0){
			if(argc==4){
				strcpy(jugador_a_eliminar,argv[3]);
				eliminar=true;
			}
			else{
				perror("Comando invalido");
				return -1;
			}
		}
		else{
			printf("Comando invalido");
			return 0;
		}
		FILE* lista_partidas= fopen(archivo_a_editar,"r");					
		if(!lista_partidas){
			perror("No se pudo abrir el archivo");
			return -1;
		}
		FILE* aux=fopen("aux.csv","w");	
		if(!aux){
			fclose(lista_partidas);
			perror("No se pudo realizar la accion");
			return -1;
		}

		llenar_struct_de_partidas(lista_partidas,partidas,&tope_partidas);

		if(agregar){
			agregar_partida(aux,partidas,tope_partidas);
		}
		else if(eliminar){
			eliminar_partida(aux,partidas,tope_partidas,jugador_a_eliminar);
		}
		else if(ordenar){
			ordenar_partidas(aux,partidas,tope_partidas);
		}
		fclose(lista_partidas);
		fclose(aux);
		rename("aux.csv","partidas.csv");
	}
	else{
		printf("Comando invalido o incompleto\n");
	}
	return 0;
}