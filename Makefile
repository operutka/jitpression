CC=clang
CPP=clang++
LLVM_FLAGS=`llvm-config --cppflags`
LLVM_LIBS=`llvm-config --ldflags --libs core jit native`
CFLAGS=-Wall -Wno-long-long -pedantic -std=c++11 -O3 -g ${LLVM_FLAGS}
CLIBS=-fopenmp ${LLVM_LIBS}
MKDIR=mkdir -p
RM=rm -rf

OBJS=	build/argtable.o \
		build/x86_64_compiler.o \
		build/llvm_compiler.o \
		build/expression.o \
		build/function.o \
		build/interpreter.o \
		build/exp_optimizer.o \
		build/exp_derivator.o \
		build/operand.o \
		build/operator.o \
		build/parser.o \
		build/token.o \
		build/tokenizer.o \
		build/test.o
HS=		argtable.h \
		x86_64_compiler.h \
		llvm_compiler.h \
		expression.h \
		function.h \
		interpreter.h \
		exp_optimizer.h \
		exp_derivator.h \
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
	${CPP} ${CFLAGS} -o $@ -c $< ${INCLUDE}

build/x86_64_compiler.o: x86_64_compiler.cpp
	${CPP} ${CFLAGS} -o $@ -c $< ${INCLUDE}

build/llvm_compiler.o: llvm_compiler.cpp
	${CPP} ${CFLAGS} -o $@ -c $< ${INCLUDE}

build/expression.o: expression.cpp
	${CPP} ${CFLAGS} -o $@ -c $< ${INCLUDE}

build/function.o: function.cpp
	${CPP} ${CFLAGS} -o $@ -c $< ${INCLUDE}

build/interpreter.o: interpreter.cpp
	${CPP} ${CFLAGS} -o $@ -c $< ${INCLUDE}

build/exp_optimizer.o: exp_optimizer.cpp
	${CPP} ${CFLAGS} -o $@ -c $< ${INCLUDE}

build/exp_derivator.o: exp_derivator.cpp
	${CPP} ${CFLAGS} -o $@ -c $< ${INCLUDE}

build/operand.o: operand.cpp
	${CPP} ${CFLAGS} -o $@ -c $< ${INCLUDE}

build/operator.o: operator.cpp
	${CPP} ${CFLAGS} -o $@ -c $< ${INCLUDE}

build/parser.o: parser.cpp
	${CPP} ${CFLAGS} -o $@ -c $< ${INCLUDE}

build/token.o: token.cpp
	${CPP} ${CFLAGS} -o $@ -c $< ${INCLUDE}

build/tokenizer.o: tokenizer.cpp
	${CPP} ${CFLAGS} -o $@ -c $< ${INCLUDE}

build/test.o: test.cpp
	${CPP} ${CFLAGS} -o $@ -c $< ${INCLUDE}
	
clean:
	${RM} build dist
	
${OBJS}: ${HS}

prepare:
	if [ ! -d build ]; then ${MKDIR} build; fi
	if [ ! -d dist ]; then ${MKDIR} dist; fi

