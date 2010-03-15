

mdistor: main.o 
	gcc -o m_distortion `pkg-config --cflags --libs jack` main.c

clean:
	rm $(OBJS) main.o m_distortion 
