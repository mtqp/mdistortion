main : main_dicc_arb.c  dicc_arb_bin.o
	gcc -o dicc_arb main_dicc_arb.c  dicc_arb_bin.o

dicc_arb_bin.o : dicc_arb_bin.asm
	nasm -g -f elf dicc_arb_bin.asm

clean : 
	rm *.o
