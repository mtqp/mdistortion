MATH = -lm
NASM = -f elf
DEBUGNASM = -g
DEBUGC = -ggdb
CFLAGS = -Wall

fact: factorial.c mathsFunctions.o
	gcc -o fact factorial.c mathsFunctions.o $(MATH)

%.o: %.c
	gcc -c $< $(DEBUGC) $(CFLAGS) $(MATH)

	
%.o : %.asm
	nasm $(DEBUGNASM) $(NASM) $< 

clean:
	rm -f fact 
	rm -f *.o
