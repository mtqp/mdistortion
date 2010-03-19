main : main.c  ejer_7.o
	gcc -ggdb -o main main.c  ejer_7.o

ejer_7.o : ejer_7.asm
	nasm -g -f elf ejer_7.asm
	
clean : 
	rm *.o
