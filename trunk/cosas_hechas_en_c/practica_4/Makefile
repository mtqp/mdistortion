main : main.c ejer_1.o ejer_3.o
	gcc -o main main.c ejer_1.o ejer_3.o

ejer_1.o : ejer_1.asm
	nasm -g -f elf ejer_1.asm

ejer_3.o : ejer_3.asm
	nasm -g -f elf ejer_3.asm
	
clean : 
	rm *.o
