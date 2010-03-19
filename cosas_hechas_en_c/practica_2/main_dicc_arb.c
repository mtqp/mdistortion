#include <stdio.h>
#include <stdlib.h>
#include <string.h>		//ESTO ES DE C

/////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _nodo {
    char* clave;
    char* significado;
    struct _nodo* izq;     /* puntero al arbol izquierdo */
    struct _nodo* der;    /* puntero al arbol derecho */
} nodo;

typedef struct _dicc_arb_bin {
    nodo* raiz;
} dicc_arb_bin;
/////////////////////////////////////////////////////////////////////////////////////////////////////
// int strcmp( const char *str1, const char *str2 );

extern void agregarPalabra(dicc_arb_bin* d, char* clave, char* significado);
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
int main(){
	srand(time(NULL));
	int i;
	char* clave = "a";
	char* significado = "sarasa";
	char* clave2 = "b";
	char* significado2 = "sarasa^2";
	printf("antes de siquiera respirar\n");
	dicc_arb_bin diccAB;
	diccAB.raiz=NULL;		//lo inicializo
	printf("casteado a int la raiz... %d %x\n",(int) diccAB.raiz, (int)&diccAB);
	agregarPalabra(&diccAB,clave,significado);
	printf("clave: %s significado:%s\n",(diccAB.raiz)->clave,diccAB.raiz->significado);
	agregarPalabra(&diccAB,clave2,significado2);
	printf("sadas\n");
	printf("ptr_der == %d\n",(int) (diccAB.raiz)->der);
	printf("ptr_izq == %d\n",(int) (diccAB.raiz)->izq);
	//printf("clave: %s significado:%s\n",(diccAB.raiz)->der->clave,diccAB.raiz->der->significado);
	//printf("clave: %s\n",(diccAB.raiz)->der->clave);
	return 0;
}
