main : main.c  ejer_8.o
	gcc -o main main.c  ejer_8.o

ejer_1.o : ejer_1.asm
	nasm -g -f elf ejer_1.asm

ejer_8.o : ejer_8.asm
	nasm -g -f elf ejer_8.asm
	
clean : 
	rm *.o
