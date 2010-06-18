OBJS =  m_window.o callbacks.o globals.o
JACK = `pkg-config --cflags --libs jack`
GTK  = `pkg-config --cflags --libs gtk+-2.0 gmodule-2.0`
MATH = -lm
NASM = -g -f elf
CFLAGS = -Wall

mdistor: main.c $(OBJS)
	gcc -o m_distortion $(GTK) $(MATH) main.c $(OBJS)

%.o: %.c
	gcc -c $< $(GTK) $(MATH)

%.o : %.asm
	nasm $(NASM) $< 

clean:
	rm -f m_distortion
	rm -f *.o
