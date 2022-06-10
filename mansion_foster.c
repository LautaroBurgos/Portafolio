#include"mansion_foster.h"

void llenar_struct_de_partidas(FILE* lista_partidas,partida_t partidas[MAX_PARTIDAS],int* tope_partidas){
	char gano[10];
	while(!feof(lista_partidas)){
		fscanf(lista_partidas,LEER_PARTIDAS,partidas[*tope_partidas].jugador,&partidas[*tope_partidas].nivel_llegado,&partidas[*tope_partidas].puntos,&partidas[*tope_partidas].vidas_restantes,gano);
		if(strcmp(gano,SI)==0){
			partidas[*tope_partidas].gano=true;
		}
		else{
			partidas[*tope_partidas].gano=false;
		}
		(*tope_partidas)++; 
	}
}
bool es_nivel_valido(int nivel){
	if((nivel==NIVEL_1)||(nivel==NIVEL_2)||(nivel==NIVEL_3)){
		return true;
	}
	else{
		return false;
	}
}
bool es_vida_valida(int vidas){
		if((vidas==0)||(vidas==1)||(vidas==2)||(vidas==3)){
		return true;
	}
	else{
		return false;
	}	
}
bool es_formato_valido(char formato[10]){
	if((strcmp(formato,SI)==0)||(strcmp(formato,NO)==0)){
		return true;
	}
	else{
		return false;
	}
}
void ingresar_datos_de_partida(partida_t* partida_a_agregar){
	char nombre[MAX_NOMBRE];
	char gano[10];
	printf("Ingrese nombre de jugador(SIN ESPACIOS)\n");
	scanf("%s",nombre);
	strcpy(((*partida_a_agregar).jugador),nombre);
	while(!es_nivel_valido((*partida_a_agregar).nivel_llegado)){
		printf("Ingrese el nivel al que llego(1,2,o 3)\n");
		scanf("%i",&((*partida_a_agregar).nivel_llegado));
	}	
	printf("Ingrese cuantos puntos logro\n");
	scanf("%i",&((*partida_a_agregar).puntos));

	printf("Ingrese cuantas vidas restantes tiene(0,1,2,o 3)\n");
	scanf("%i",&((*partida_a_agregar).vidas_restantes));
	while(!es_vida_valida((*partida_a_agregar).vidas_restantes)){
		printf("Ingrese cuantas vidas restantes tiene(1,2,o 3)\n");
		scanf("%i",&((*partida_a_agregar).vidas_restantes));
	}

	while(!es_formato_valido(gano)){
		printf("Ingrese si gano o no(Formato : Si o No)\n");
		scanf("%s",gano);
		if(strcmp(gano,SI)==0){
			(*partida_a_agregar).gano=true;
		}
		else if(strcmp(gano,NO)==0){
			(*partida_a_agregar).gano=false;
		}
	}
}
void agregar_partida(FILE* aux,partida_t partidas[MAX_PARTIDAS],int tope_partidas){
	partida_t partida_a_agregar;
	ingresar_datos_de_partida(&partida_a_agregar);
	char gano[MAX_NOMBRE];
	bool agrego=false;

	for(int i=0;i<tope_partidas;i++){
		if(strcmp(partidas[i].jugador,partida_a_agregar.jugador)>=0&&(!agrego)){
			if(partida_a_agregar.gano){
				strcpy(gano,SI);
			}
			else{
				strcpy(gano,NO);
			}
			fprintf(aux,ESCRIBIR_PARTIDAS,partida_a_agregar.jugador,partida_a_agregar.nivel_llegado,partida_a_agregar.puntos,partida_a_agregar.vidas_restantes,gano );
			agrego=true;
		}
		if(partidas[i].gano){
			strcpy(gano,SI);
		}
		else{
			strcpy(gano,NO);
		}
		fprintf(aux,ESCRIBIR_PARTIDAS,partidas[i].jugador,partidas[i].nivel_llegado,partidas[i].puntos,partidas[i].vidas_restantes,gano );
	}
	if(!agrego){
		if(partida_a_agregar.gano){
			strcpy(gano,SI);
		}
		else{
			strcpy(gano,NO);
		}
		fprintf(aux,ESCRIBIR_PARTIDAS,partida_a_agregar.jugador,partida_a_agregar.nivel_llegado,partida_a_agregar.puntos,partida_a_agregar.vidas_restantes,gano );
	}
}
void eliminar_partida(FILE* aux,partida_t partidas[MAX_PARTIDAS],int tope_partidas,char jugador_a_eliminar[MAX_NOMBRE]){
	bool elimino=false;
	char gano[10];
	for(int i=0;i<tope_partidas;i++){
		if(strcmp(partidas[i].jugador,jugador_a_eliminar)!=0||((strcmp(partidas[i].jugador,jugador_a_eliminar)==0)&&(elimino))){
			if(partidas[i].gano){
				strcpy(gano,SI);
			}
			else{
				strcpy(gano,NO);
			}
			fprintf(aux,ESCRIBIR_PARTIDAS,partidas[i].jugador,partidas[i].nivel_llegado,partidas[i].puntos,partidas[i].vidas_restantes,gano );
		}
		else{
			elimino=true;
		}
	}
}
void ordenar_aux(partida_t partidas[MAX_PARTIDAS],int tope_partidas){
	partida_t aux;
	for(int i=1;i<tope_partidas;i++){
		for(int j=0;j<tope_partidas-1;j++){
			if(strcmp(partidas[j].jugador,partidas[j+1].jugador)>0){
				aux=partidas[j];
				partidas[j]=partidas[j+1];
				partidas[j+1]=aux;
			}
		}
	}	
}
void ordenar_partidas(FILE* aux,partida_t partidas[MAX_PARTIDAS],int tope_partidas){
	char gano[10];
	ordenar_aux(partidas,tope_partidas);
	for(int i=0;i<tope_partidas;i++){
			if(partidas[i].gano){
				strcpy(gano,SI);
			}
			else{
				strcpy(gano,NO);
			}
		fprintf(aux,ESCRIBIR_PARTIDAS,partidas[i].jugador,partidas[i].nivel_llegado,partidas[i].puntos,partidas[i].vidas_restantes,gano );
	}
}