#include <stdio.h>
#include <stdlib.h>
//ejercicio 1
extern float suma(float,float);
extern float sumaDouble(double,double);
extern double mult_float(double,double);
extern double area_circ(double);

//ejercicio 3
extern double potencia(double, int);
extern int bashkara(double a, double b, double c, double * r1, double * r2);

void potenciaC (double a, int n, double * res){
	double avar = a;
	while (n > 1){
		avar = a*avar;
		n--;
	}
	printf("x aqui paso\n");
	res[0] = avar;
}


//ejercicio 9
extern unsigned fib(unsigned n);

int main(){
	unsigned res;
	unsigned i;
	double * pot = malloc(8);
	for(i=1;i<=10;i+=1){
		res = fib(i);
		printf("el fibonacci de %d es: %d\n",i,res);
//		potenciaC(2.0f,i,pot);	
//		printf("el potenciaC de %d es: %lf\n",i,*pot);
	}	
	
	/*int res;
	double *r1 = malloc(8);
	double *r2 = malloc(8);
	res = bashkara(1.0f,-3.0f,2.0f,r2,r1);
	
	if (res) {
		printf("tiene 2 raices reales!\n");
		printf("r1 == %lf\nr2 == %lf\n",r1[0],r2[0]);
	//printf("%lf",r2[0]);
	}
	else {
		printf("no tiene las dos raices reales, o no es cuadratica!\n");
	}
	//free(r1);
	//free(r2);
	*/
	/*float res;
	res = potencia(2.0f,5);
	printf("la potencia de float a la int da: %f\n",res);
	res = suma(5.0f,5.0f);
	printf("La suma en floats DA: %f\n",res);
	double resDouble;
	resDouble = sumaDouble(3.14f,10.0f);
	printf("La suma en floats doubles DA: %lf\n",resDouble);
	double res_Mult;
	res_Mult = mult_float(1.41f,1.41f);
	printf("La multiplicacion en floats doubles DA: %lf\n",res_Mult);
	double res_area;
	res_area = area_circ(1.0f);
	printf("El area de un circ de radio 1 es: %lf\n",res_area);
	*/
	
	return 0;
}
