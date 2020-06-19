CFLAGS=-O2 -Wall -Wpedantic -Wunused-function -lX11 -lXrandr -lXrender -lXi -lXfixes -lGL -lIL -lILU -lILUT `pkg-config --libs --cflags mpv` -g

.PHONY : all
all:
	${CC} ${CFLAGS} *.c -o main

run : all
	@exec ./main

.PHONY : ctags
ctags:
	ctags -R --exclude=.git -f .tags .
