#include <stdio.h>
#include <stdlib.h>

typedef struct _nodo {
	int dato;
	struct _nodo* ptr_sig;
}nodo;
typedef struct _lista{
	nodo* primero;
	nodo* ultimo;
}lista;

extern void agregarAD(lista * l, int n);
extern void agregarAT(lista * l,int n);
extern int buscar(lista * l, int n);
extern void eliminarN(lista *l, int);
extern void inicializar(lista * l);
extern int iesimo(lista * l, int n); //precond n<tam(lista) no tengo iesimo([])
//pensar un printF mas eficiente q buscar iesimo!
int main(){
	srand(time(NULL));
	int i = 0;
	lista l1;
	inicializarL(&l1);
	/*agregarAD(&l1,4);
	printf("%d \n",iesimo(&l1,0));
	printf("llamo a eliminar el 4 \n");
	eliminarN(&l1,4);*/
	//printf("printeo el iesimo%d \n",iesimo(&l1,0));


	/*
	agregarAT(&l1,4);
	agregarAT(&l1,5);
	printf("OK");
	//iesimo(&l1,0);
		printf("OK");*/
	//iesimo(&l1,1);
	//printf("debe dar 4 %d \n",iesimo(&l1,1));
	
	for(i=0;i<10;i+=1){
	//	agregarAT(&l1,i%10);
	//	printf("ok \n");
		agregarAD(&l1,i%9);
	}
	//agregarAD(&l1,3);
	//buscar(&l1,0);
	//printf("pos de buscar numero_%d es: %d \n",1,buscar(&l1,1));
	//printf("pos de buscar numero_%d es: %d \n",2,buscar(&l1,2));
	for(i=0;i<10;i+=1){
		printf("iesimo %d es %d \n",i,iesimo(&l1,i));
	}
	for(i=0;i<20;i+=1){
		printf("trato eliminar elem == %d --> \n",i%10);
		eliminarN(&l1,i%10);
	//	printf("pos sub_%d es: %d \n",i,buscar(&l1,rand()%100));
	//	printf("pos sub_%d es: %d \n",i,iesimo(&l1,i));
	}
	//printf("elemento 3 pos 0 es: %d \n",buscar(&l1,3));
	
	return 0;
}
