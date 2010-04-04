OBJS = m_distortion.o m_distortion_ch.o callbacks.o
JACK = `pkg-config --cflags --libs jack`
GTK  = `pkg-config --cflags --libs gtk+-2.0 gmodule-2.0`
MATH = -lm

mdistor: main.c $(OBJS)
	gcc -o m_distortion $(JACK) $(GTK) $(MATH) main.c $(OBJS)

%.o: %.c
	gcc -c $< $(JACK) $(GTK) $(MATH)

clean:
	rm -f m_distortion
	rm -f *.o
