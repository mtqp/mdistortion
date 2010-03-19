main : main_conjunto.c  conjunto.o
	gcc -o conjunto main_conjunto.c  conjunto.o

conjunto.o : conjunto.asm
	nasm -g -f elf conjunto.asm

clean : 
	rm *.o
