PACKAGES = xcb

CFLAGS = $(shell pkg-config --cflags $(PACKAGES)) -Wall -Wextra -std=c99 -pedantic -g
LDFLAGS = $(shell pkg-config --libs $(PACKAGES))

all: irssi-focus

irssi-focus: irssi-focus.o atoms.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm -f *.o irssi-focus
