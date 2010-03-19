#include <stdio.h>
#include <math.h>	//para q funcione el rand
#include <stdlib.h>

//daltan todos los externs q los borre

/*
el says:
    [17:37:47] ­elemento por elemento
    [17:38:01] ­num8 miArray[8];
    [17:38:39] ­miArray[0].valido = 0;
miArray[0].dato = 123;
    [17:38:41] ­o sino
    [17:39:18] ­miArray[0]=(num8){(char)0, (char)123 };
­Mariano says:
*/
int main(){
	srand(time(NULL));
	int i=0;
	unsigned char elem;
	int random;
	printf("WHAT THE FUCKING FUCK?\n");
	num8 arrayV[256];
	num8 arrayVRES[256];
	printf("entro al primer for?\n");
	for (i;i<256;i+=1){
		elem = (unsigned char)rand()%17;
		arrayV[i].dato = elem;
		arrayVRES[i].dato = elem;
		random = rand()%255;
		if (random>128) {
			arrayV[i].valido = (char) 0;
			arrayVRES[i].valido = (char) 0;
		}
		else {
			arrayV[i].valido = (char) 0xFF;
			arrayVRES[i].valido = (char) 0xFF;
		}
	}
	printf("hizo los dos arrays V y VRES\n");
	num8 arrayW[256];
	for (i=0;i<256;i+=1){
		elem = (unsigned char) rand()%17;
		arrayW[i].dato = elem;
		random = rand()%255;
		if (random>128) arrayW[i].valido = (char) 0;
		else arrayW[i].valido = (char) 0xFF;
	}
	
	printf("multiplicando alocamente\n");
	multiplicarSIvalido(arrayVRES, arrayW, 256);
	for (i=0;i<30;i+=1){
		printf("arrayV V=%d \tD=%d, arraW \t V=%d  \t D=%d, \t RES V=%d \t D=%d\n",(int)arrayV[i].valido,(int)arrayV[i].dato,(int)arrayW[i].valido,(int)arrayW[i].dato,(int)arrayVRES[i].valido,(int)arrayVRES[i].dato);
	}



	/*int * promr = malloc(4);
	int * varianza = malloc(4);
	short int* vector = malloc(16);
	for(i;i<8;i+=1){
		vector[i]=rand()%30;
	}
	prom(vector,promr,varianza);
	printf("el promedio da: %d \n la varianza da: %d \n",promr[0],varianza[0]);
		*//*
	short int * v1 = malloc(128);	//64 nros de 16 bits
	char* v2 = malloc(64);		//64 nros de 8 bits
	unsigned short tam_prod = 64;	//tamaño array
	for(i;i<tam_prod;i+=1){
		v1[i]=rand()%4;
		v2[i]=rand()%4;
	}
	int* res_prod = prodInterno(v1,v2,tam_prod);
	printf("Prod interno de 64 numeros \n");
	for(i=0;i<tam_prod;i+=1){
		printf("La multiplicacion sub_%hd da: %d \n ",i,res_prod[i]);
	}
		*//*
	short int * v = malloc(48);
	short int * w = malloc(48);	//tengo 24 elementos para meter
	short int cargar_elementos = 0;
	int tam = 24;			//asi hace como 14 pasadas
	i=0;
	for(cargar_elementos;cargar_elementos<tam;cargar_elementos+=1){
		v[cargar_elementos]=3;
		w[cargar_elementos]=4;
	}
	short int * res_subi = sumaYResta(v,w,tam);
	printf("Las posiciones multiplo de dos dan 7 ,\n las impares -1\n");
	for(i;i<tam;i+=1){
		printf("la componente subI  %d da: %hd\n ",i,(int)res_subi[i]);
	}
	
	for(cargar_elementos;cargar_elementos<tam;cargar_elementos+=1){
		v[cargar_elementos]=rand()%10;
		w[cargar_elementos]=rand()%10;
	}
	short int * rand_res_subi = sumaYResta(v,w,tam);
	printf("\n\n test con numeros random!!!");
	printf("LAS POSICIONES PARES SE SUMAN ,\n LAS IMPARES SE RESTAN\n");
	i=0;
	for(i;i<tam;i+=1){
		printf("la componente subI  %d da: %hd\n ",i,(int)rand_res_subi[i]);
	}
	
	free(v);
	free(w);
	free(res_subi);
	free(rand_res_subi);
	*/
	/*
	free(v1);
	free(v2);
	free(res_prod);
	*/
	return 0;
}
