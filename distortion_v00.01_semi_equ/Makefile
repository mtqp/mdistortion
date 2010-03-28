OBJS = m_distortion.o m_distortion_ch.o m_equalizer.o

mdistor: main.c $(OBJS)
	gcc -o m_distortion `pkg-config --cflags --libs jack` -lm main.c $(OBJS)

clean:
	rm -f m_distortion
	rm -f *.o
