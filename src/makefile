# Menu Navigator Makefile

CC=gcc
CFLAGS=
INCLUDES=
LIBS=-lncurses -ltinfo
TARGETS=test_out main.o menu.o menu.h.gch config.o config.h.gch input.o input.h.gch errors.o errors.h.gch

test_out: main.o menu.o menu.h config.o config.h input.o input.h errors.o errors.h
	${CC} ${CFLAGS} -o test_out main.o menu.o menu.h config.o config.h input.o input.h errors.o errors.h ${LIBS}

main.o: main.c
	${CC} ${CFLAGS} -c main.c menu.h config.h input.h ${LIBS}

menu.o: menu.c
	${CC} ${CFLAGS} -c menu.c ${LIBS}

config.o: config.c
	${CC} ${CFLAGS} -c config.c ${LIBS}

input.o: input.c errors.h
	${CC} ${CFLAGS} -c input.c input.h errors.h ${LIBS}

errors.o: errors.c
	${CC} ${CFLAGS} -c errors.c errors.h ${LIBS}

clean:
	rm ${TARGETS}
