AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libjitpression.la
bin_PROGRAMS = test

libjitpression_la_CXXFLAGS = -Wall -Wno-long-long -pedantic -std=c++11 `llvm-config --cppflags`
libjitpression_la_LDFLAGS = `llvm-config --ldflags --libs core jit native`
libjitpression_la_SOURCES = argtable.h argtable.cpp \
		compiler.h \
		x86_64_compiler.h x86_64_compiler.cpp \
		llvm_compiler.h llvm_compiler.cpp \
		expression.h expression.cpp \
		function.h function.cpp \
		functor.h functor.cpp \
		interpreter.h interpreter.cpp \
		exp_optimizer.h exp_optimizer.cpp \
		exp_derivator.h exp_derivator.cpp \
		exp_printer.h exp_printer.cpp \
		operand.h operand.cpp \
		operator.h operator.cpp \
		opvisitor.h \
		parser.h parser.cpp \
		token.h token.cpp \
		tokenizer.h tokenizer.cpp \
		context.h context.cpp \
		jitpression.h jitpression.cpp

test_CFLAGS = -Wall -Wno-long-long -pedantic -fopenmp
test_LDFLAGS = -ljitpression
test_SOURCES = test.c
