OBJS = m_distortion.o m_distortion_ch.o 
JACK = `pkg-config --cflags --libs jack`
GTK  = `pkg-config --cflags --libs gtk+-2.0 gmodule-2.0`
MATH = -lm

mdistor: main.c $(OBJS)
	gcc -o m_distortion $(JACK) $(GTK) $(MATH) main.c $(OBJS)

clean:
	rm -f m_distortion
	rm -f *.o
	
	
#http://library.gnome.org/devel/gtk-tutorial/2.17/c491.html
#http://library.gnome.org/devel/gtk-tutorial/2.17/x647.html
#http://library.gnome.org/devel/gtk-tutorial/2.17/x1186.html
#http://library.gnome.org/devel/gtk-tutorial/2.17/c1501.html
