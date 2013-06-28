CC=gcc
CPP=g++
CFLAGS=-m64 -Wall -Wno-long-long -pedantic -std=c++11 -O3 -g
CLIBS=-fopenmp
MKDIR=mkdir -p
RM=rm -rf

OBJS=	build/argtable.o \
		build/compiler.o \
		build/expression.o \
		build/function.o \
		build/interpreter.o \
		build/operand.o \
		build/operator.o \
		build/parser.o \
		build/token.o \
		build/tokenizer.o \
		build/test.o
HS=		argtable.h \
		compiler.h \
		expression.h \
		function.h \
		interpreter.h \
		operand.h \
		operator.h \
		opvisitor.h \
		parser.h \
		token.h \
		tokenizer.h

INCLUDE=
OUT_DIR=dist
OUT_FILE=${OUT_DIR}/test

build: prepare link

link: ${OBJS}
	${CPP} ${CFLAGS} ${OBJS} -o ${OUT_FILE} ${CLIBS}

build/argtable.o: argtable.cpp
	${CPP} ${CFLAGS} -o $@ -c $< ${CLIBS} ${INCLUDE}

build/compiler.o: compiler.cpp
	${CPP} ${CFLAGS} -o $@ -c $< ${CLIBS} ${INCLUDE}

build/expression.o: expression.cpp
	${CPP} ${CFLAGS} -o $@ -c $< ${CLIBS} ${INCLUDE}

build/function.o: function.cpp
	${CPP} ${CFLAGS} -o $@ -c $< ${CLIBS} ${INCLUDE}

build/interpreter.o: interpreter.cpp
	${CPP} ${CFLAGS} -o $@ -c $< ${CLIBS} ${INCLUDE}

build/operand.o: operand.cpp
	${CPP} ${CFLAGS} -o $@ -c $< ${CLIBS} ${INCLUDE}

build/operator.o: operator.cpp
	${CPP} ${CFLAGS} -o $@ -c $< ${CLIBS} ${INCLUDE}

build/parser.o: parser.cpp
	${CPP} ${CFLAGS} -o $@ -c $< ${CLIBS} ${INCLUDE}

build/token.o: token.cpp
	${CPP} ${CFLAGS} -o $@ -c $< ${CLIBS} ${INCLUDE}

build/tokenizer.o: tokenizer.cpp
	${CPP} ${CFLAGS} -o $@ -c $< ${CLIBS} ${INCLUDE}

build/test.o: test.cpp
	${CPP} ${CFLAGS} -o $@ -c $< ${CLIBS} ${INCLUDE}
	
clean:
	${RM} build dist
	
${OBJS}: ${HS}

prepare:
	if [ ! -d build ]; then ${MKDIR} build; fi
	if [ ! -d dist ]; then ${MKDIR} dist; fi

