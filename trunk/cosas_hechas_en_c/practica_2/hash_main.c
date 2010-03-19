#include <stdio.h>
#include <stdlib.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _nodo{
	int valor;
	struct _nodo* ptr_prox;
}nodo;

typedef struct _hash{
	nodo* array_hash[20];
}hash;
/////////////////////////////////////////////////////////////////////////////////////////////////////

extern nodo * recorrer_nodos_final(nodo* n);
extern int seleccion_array_subi(int n);
extern void agregarH(hash *hs,int n);			//preguntar en clase xq no anda esto
extern int estaH(hash* hs,int n);
extern void eliminarH(hash *hs, int n);
//extern int promedioH(hash *h);

/////////////////////////////////////////////////////////////////////////////////////////////////////
void inicializarH(hash * ptr_hs){
	int i=0;
	for(i;i<20;i+=1){
		ptr_hs->array_hash[i] = NULL;
	}
}
void cAgregar(hash * ptr_hs,int n){
	nodo * ptr_nnew = malloc(8);
	ptr_nnew->valor=n;
	ptr_nnew->ptr_prox=NULL;
	int corrimiento = seleccion_array_subi(n);
	if (ptr_hs->array_hash[corrimiento]==NULL) {
		(ptr_hs->array_hash[corrimiento])=ptr_nnew;
		//printf("entrando a un nuevo bucket \n");
	}
	else{
		(recorrer_nodos_final(ptr_hs->array_hash[corrimiento]))->ptr_prox= ptr_nnew;
		//printf("recorrio nodos \n");

	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
int main(){
	srand(time(NULL));
	int i;
	hash hs;
	inicializarH(&hs);

	for(i=-1000;i<=1000;i+=5){
		//printf("%d",i);
		cAgregar(&hs,i);
	}
/*
	for(i=-1000;i<=1000;i+=25){
		if(estaH(&hs,i)){
			printf("existe el elem == %d \n",i);
		} else {
			printf("no existe el elem == %d \n",i);
		}

	}*/
	
	printf("eliminar solo avisa si elem no existe \n");
	for(i=100;i<1000;i+=1){
		printf("%d ",i);
		eliminarH(&hs,i);
		printf("\n");
	}
	//agregarH(&hs,12);
	//printf("dato cero, bucket 10:  %d \n",((&hs)->array_hash[10])->valor);
/*	for (i=-1000;i<=1000;i+=25){
		cAgregar(&hs,i);
		printf("agrego sin seg %d \n",i);
		//printf("numero_%d en bucket %d \n",i,seleccion_array_subi(i));
	}
*/	return 0;
}
