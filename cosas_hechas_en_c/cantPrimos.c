#include <stdio.h>
#include <string.h>

extern ListaPrimos* primosDivisoresA (int a)
extern q te devuelve!?!?cantPrimos(int a)
extern void agregarN(int dato, ListaPrimos* lp)
extern int damePos(int subI, ListaPrimos* lp)

struct NodoPrimos{
	int dato;
	Nodo* siguiente;
};

struct ListaPrimos  {
	Nodo* comienzo;		
	Nodo* fin; 
	int tam;
};

void inicializarLP(ListaPrimos* lp){
	lp->comienzo = NULL;
	lp->fin = NULL;
	lp->tam = 0;
}
bool listaVacia (ListaPrimos* lp){
	return lp->comienzo == NULL   ; esto debe franquear paso un puntero...
}

bool esUltimo (NodoPrimos* n){
	return n->siguiente == NULL;
}



void  main(){
	ListaPrimos lp;
	
	primosDivisoresA (100);	
	
	
}
