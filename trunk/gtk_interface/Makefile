#OBJS = sarasa

main: main.c
	gcc -o gtk_interface `pkg-config --cflags --libs gtk+-2.0` main.c

clean:
	rm -f gtk_interface
	rm -f *.o
	
	
#	http://library.gnome.org/devel/gtk-tutorial/2.17/x159.html
