CC=gcc
CFLAGS=-g -O2

DEPS = models.h dataset.h
OBJS = models.o dataset.o main.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

main: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf *.o main 
