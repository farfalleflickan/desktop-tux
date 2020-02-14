CFLAGS=-O2 -Wall -lX11 -lXpm -lXrandr -lXrender -lGL -lm


.PHONY : all
all:
	${CC} ${CFLAGS} *.c -o main
	
run : all
	@exec ./main
