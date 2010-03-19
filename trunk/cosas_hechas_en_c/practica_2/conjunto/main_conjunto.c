#include <stdio.h>
#include <stdlib.h>
#include <string.h>		//ESTO ES DE C

/////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _nodo
{
    int elem;
    struct _nodo* siguiente;
} nodo;

typedef struct _conj
{
    nodo* primero;
} conj;

void  dum_sort(int* ptr_array,int tam){
	int i;
	int dato_prov;
while(!esta_ordenado(ptr_array,tam)){
	i = 0;
	for(i;i<tam;i+=1){
		if(ptr_array[i]>ptr_array[i+1] && !(i+1==tam)){
			dato_prov 	 = ptr_array[i];
			ptr_array[i]	 = ptr_array[i+1];
			ptr_array[i+1]	 = dato_prov;
		}
	}
}
}

int esta_ordenado(int* ptr_array, int tam){
	int i;
	for (i=0; i<tam; i+=1){
		if (!(ptr_array[i] <= ptr_array[i+1])&& !(i+1==tam)){
			return 0;
		}
	}
	return 1;
}



extern int esta(conj* c, int elem);
extern void agregarElemento(conj* c, int elem);
/////////////////////////////////////////////////////////////////////////////////////////////////////
int main(){
	srand(time(NULL));
	int i;
	int cantELEM = 20;
	int modRAND = 30;
	conj c;
	c.primero = NULL;				//lo inicializo
	int random;
	char* respuesta;
	int* conjuntoC = malloc(4*cantELEM);
	for(i=0;i<cantELEM;i+=1){
		random = rand()%modRAND;
		conjuntoC[i] = random;
		agregarElemento(&c,random);
		printf("ag elem %d\n",random);
	}
	dum_sort(conjuntoC, cantELEM);
	for(i=0;i<cantELEM;i+=1){
		printf("C.conjORD_%d == %d ---- ASM.conjORD_%d == %d\n",i,conjuntoC[i],i,c.primero->elem);
	}
	for(i=0;i<cantELEM;i+=1){
		random = rand()%modRAND;
		if(esta(&c,random)) respuesta = "SI!";
		else respuesta = "NO che, no está!";
		printf("esta %d?? %s\n",random,respuesta);
	}
	return 0;
}
