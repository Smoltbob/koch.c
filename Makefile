CC = clang
LDFLAGS = -lm
CFLAGS = -std=c99 -Wall -Wextra -g

all: koch

koch: koch_main.o koch_fonctions.o koch_ihm.o create_image.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o *~ koch *.ppm

