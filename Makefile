all: irssi-focus irssi-focus-2

irssi-focus: irssi-focus..1.c
	gcc -Wall -pedantic -o $@ $^ -lX11 $(shell pkg-config --cflags --libs glib-2.0) -Os -fno-strict-aliasing -fomit-frame-pointer

irssi-focus-2: irssi-focus.c
	gcc -Wall -std=c99 -pedantic -o $@ $^ $(shell pkg-config --cflags --libs xcb) -g

clean:
	rm -f irssi-focus irssi-focus-2
