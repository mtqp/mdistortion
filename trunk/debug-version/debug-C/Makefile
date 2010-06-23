OBJS =  m_window.o m_distortion.o m_effects.o m_hall.o m_delay.o m_eq.o callbacks.o vol_ctes.o dummy_audio.o m_user_interface.o
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
