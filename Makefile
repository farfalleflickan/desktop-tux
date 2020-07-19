CFLAGS=-O2 -Wall -Wpedantic -Wunused-function -lX11 -lXrandr -lXrender -lXi -lXfixes -lGL -lIL -lILU -Wl,-rpath='./' -L./ -lILUT -lSDL -lSDL_mixer -lmpv -g

.PHONY : all
all: ctags
	${CC} ${CFLAGS} *.c -o main

run : all
	@exec ./main

.PHONY : ctags
ctags:
	ctags -R --exclude=.git -f .tags .
