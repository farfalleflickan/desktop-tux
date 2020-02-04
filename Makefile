CFLAGS=-O2 -Wall -lX11 -lXi -lXpm -lXext

.PHONY : all
all:
	${CC} ${CFLAGS} *.c -o main
	
run : all
	@exec ./main
