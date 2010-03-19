main : main.c  ejer_10.o
	gcc -o main main.c  ejer_10.o

ejer_10.o : ejer_10.asm
	nasm -g -f elf ejer_10.asm
	
clean : 
	rm *.o
