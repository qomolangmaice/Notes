CC = gcc
CFLAGS = -Wall -g -gdwarf-2 -gstrict-dwarf -O0
SRC = $(wildcard *.c)
OBJS = $(patsubst %.c, %, $(SRC))

all: $(OBJS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

insight:
	insight $(OBJS)

clean:
	rm $(OBJS) .*.sw?

