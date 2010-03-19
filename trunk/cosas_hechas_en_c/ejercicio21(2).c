#include <stdio.h>
extern long long int suma (long long int a, long long int b);

main() {
	long long int res = 
suma(0x1000000000000001LL,0x0000000000000001LL);
	printf("%lld\n",res);

}
