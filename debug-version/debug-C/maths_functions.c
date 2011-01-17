#include "maths_functions.h"

int factorial(unsigned int x){
	if (x==0) return 1;
	unsigned int i=x-1;
	unsigned int ret=x;
	for(i;i>0;i--){
		ret*=i;
	}
	return ret;
}

/*Mejor que C... =O*/
float taylorSin(float x){
	float positivos;
	float negativos;
	
	positivos = x + (pow(x,5)/(float)factorial(5)) + (pow(x,9)/(float)factorial(9));
	negativos = (pow(x,3)/(float)factorial(3)) + (pow(x,7)/(float)factorial(7));
	
	return positivos - negativos;
}

/*Anda GENIAL =D*/
float taylorCos(float x){
	float positivos, negativos;
	positivos = 1.0 + (pow(x,4)/(float)factorial(4)) + (pow(x,8)/(float)factorial(8));
	negativos = (pow(x,2)/2) + (pow(x,6)/(float)factorial(6));
	
	return positivos - negativos;
}

/*Tiene algo de error, la mayoria dado x taylorSin supongo*/
float taylorTan(float x){
	return taylorSin(x)/taylorCos(x);
}

/*anda muy bien, se aleja al llegar a los bordes*/
float taylorLog(float x){
	float pos,neg;
	if(x>0.0){
		pos = x-1 + pow(x-1,3)/3 + pow(x-1,5)/5 + pow(x-1,7)/7 + pow(x-1,9)/9;
		neg = pow(x-1,2)/2 + pow(x-1,4)/4 + pow(x-1,6)/6 + pow(x-1,8)/8;
		return pos - neg;
	}	
	else {
		return log(-1);
	}
}
