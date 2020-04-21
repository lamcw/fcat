CFLAGS=-Wall -Werror -pedantic

fcat: fcat.c

install: fcat
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f $< $(DESTDIR)$(PREFIX)/bin

uninstall:
	$(RM) $(DESTDIR)$(PREFIX)/bin/fcat

clean:
	$(RM) fcat

.PHONY: clean install uninstall
