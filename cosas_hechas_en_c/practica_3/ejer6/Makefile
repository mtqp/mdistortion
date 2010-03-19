main : main.c  ejer_6.o
	gcc -o main main.c  ejer_6.o

ejer_6.o : ejer_6.asm
	nasm -g -f elf ejer_6.asm
	
clean : 
	rm *.o
