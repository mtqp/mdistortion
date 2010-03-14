

distor: main.o 
	gcc -o sclient `pkg-config --cflags --libs jack` main.c

mdistor:
	gcc -o m_distortion old_main.c
clean:
	rm $(OBJS) main.o old_main.o exies_client m_distortion 
