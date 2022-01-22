.PHONY: all clean install uninstall

PREFIX = /usr/local
SRC = volumectl.c
OBJ = $(SRC:.c=.o)

all: volumectl

.c.o:
	gcc -Wall -O2 -c $<

volumectl: volumectl.o
	gcc volumectl.o -o volumectl

clean:
	rm -f volumectl $(OBJ)

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f volumectl $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/volumectl

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/volumectl

