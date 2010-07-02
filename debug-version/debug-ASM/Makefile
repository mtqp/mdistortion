cOBJS	  = m_distortion.o m_effects.o m_hall.o m_delay.o m_eq.o vol_ctes.o 
asmOBJS   = distortions.o effects.o
audioOBJS = dummy_audio.o
uiOBJS    = m_window.o callbacks.o m_user_interface.o
JACK = `pkg-config --cflags --libs jack`
GTK  = `pkg-config --cflags --libs gtk+-2.0 gmodule-2.0`
MATH = -lm
NASM = -g -f elf
CFLAGS = -Wall

mdistor: main.c $(cOBJS) $(asmOBJS) $(uiOBJS) $(audioOBJS)
	gcc -o m_distortion $(GTK) $(MATH) main.c $(cOBJS) $(asmOBJS) $(uiOBJS) $(audioOBJS)

%.o: %.c
	gcc -c $< $(GTK) $(MATH)

%.o : %.asm
	nasm $(NASM) $< 

clean:
	rm -f m_distortion
	rm -f *.o

