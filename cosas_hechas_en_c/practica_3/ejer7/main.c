#include <stdio.h>
#include <math.h>	//para q funcione el rand
#include <stdlib.h>

extern void productoMatrizVector(float* A, float* v,float* res, unsigned short n,unsigned short m);

void pMVenC(float* matrizA,float* v,float* resC,int n, int m){
	int i = 0;
	int mP = 0;
	int rP = 0;
	float resP = 0.0;
	while (mP < m*n){	//no se si va el igual, tqp
		while (i < n)	{
			resP += (matrizA[mP+i]*v[i]);
			i++;
		}
		resC[rP] = resP;
		resP = 0.0;
		rP++;
		i=0;
		mP+=n;
	}
}

int main(){
	srand(time(NULL));
	int i=0;
	int n=8;
	int m=12;
	int modRAND=2;
	int random ;
	float* matrizA = malloc(n*m);
	float* v = malloc(n);
	float* res = malloc(m);
	float* resC = malloc(m);
	for(i=0;i<n*m;i+=1){
		random = rand()%modRAND;
		matrizA[i] = 1.0;//(float) 1;//random;
	}
	for(i=0;i<n;i+=1){
		random = rand()%modRAND;
		v[i] = 1.0;//(float) 1;// random;
	}

	printf("calculando en C\n");
	pMVenC(matrizA,v,resC, n,m);
	printf("termino de calcular en C\n");
	printf("calculando en ASM\n");
	productoMatrizVector(matrizA,v,res,(unsigned short) n,(unsigned short) m);
	printf("termino de calcular en ASM\n");
	//PARTE PRINTF//
	for(i=0;i<m;i+=1){
//		printf("(resC_%d = %f)\n",i,resC[i]);
		printf("(res_%d = %f)== (resC_%d = %f)\n",i,res[i],i,resC[i]);
	}

	return 0;
}
