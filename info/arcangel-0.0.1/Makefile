ver=0.0.1
jacklibs=`pkg-config jack --libs`
jackflags=`pkg-config jack --cflags`
CXX=gcc -std=c99 -O4
LIBS=-lm $(jacklibs) -lforms -lXpm -L/usr/X11R6/lib
FLAGS=$(jackflags) -I/usr/X11R6/include/X11

arcangel: arcangel.c arc.o form.o
	$(CXX) -o arcangel form.o arcangel.c arc.o $(LIBS) $(FLAGS)

arc.o: arc.c arc.h
	$(CXX) -c arc.c $(FLAGS)

form.o: form.c form.h
	$(CXX) -c form.c


clean:
	rm -f *.o arcangel *~ *.bak arcangel*.bz2


install: arcangel
	install -m a+x arcangel /bin/.

package:
	make clean
	mkdir ../arcangel-$(ver)
	cp `ls|grep -v CVS|xargs` ../arcangel-$(ver)/.
	mv ../arcangel-$(ver) .
	tar cf arcangel-$(ver).tar arcangel-$(ver)
	bzip2 arcangel-$(ver).tar
	rm -rf arcangel-$(ver)
