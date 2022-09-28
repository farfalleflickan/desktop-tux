SHELL:=/bin/bash
CFLAGS=-O2 -Wall -Wpedantic -Wunused-function -lX11 -lXrandr -lXrender -lXi -lXfixes -lGL -lIL -lILU -Wl,-rpath='./lib' -L./lib -lILUT -lSDL -lSDL_mixer -lmpv -g

LIB_DIR=lib/
DATA_DIR=data/
BUILD_DIR=bin/
BINARY_FILE=desktop-tux
BINARY_PATH=${BUILD_DIR}${BINARY_FILE}

.PHONY : bin ctags clean valgrind
bin: ctags
	mkdir -p ${BUILD_DIR}
	${CC} src/*.c ${CFLAGS} -o ${BINARY_PATH}
	cp -r ${DATA_DIR} ${BUILD_DIR}data
	cp -r ${LIB_DIR} ${BUILD_DIR}lib

run : bin
	@exec ${BINARY_PATH}

clean:
	rm -rf ${BUILD_DIR}

ctags:
	ctags -R --exclude=.git -f .tags src/

valgrind: bin
	cd ${BUILD_DIR} && valgrind --max-threads=4096 --log-file=valgrindLog.txt --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./${BINARY_FILE} && cat valgrindLog.txt
