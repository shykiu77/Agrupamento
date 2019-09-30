SRCS = $(wildcard *.c)

PROGS = $(patsubst %.c,%,$(SRCS))

all: criar $(PROGS)

criar:
	mkdir -p Bin && mkdir -p Entradas && mkdir -p Saidas

%: %.c
	$(CC) $(CFLAGS)  -Wall -o Bin/$@ $< -lm