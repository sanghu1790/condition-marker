# Copyright 2017 National University of Singapore
#
# See LICENSE.md for license information.

CC=clang-3.4
CXX=clang++-3.4
CXXFLAGS=`llvm-config-3.4 --cxxflags` -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0
LDFLAGS=`llvm-config-3.4 --ldflags`
LDLIBS=`llvm-config-3.4 --libs` -lpthread -lncurses -ldl
EXAMPLES=tritype.bc
Z3_DIR=${HOME}/z3/build

TARGETS=markcond ${EXAMPLES}

all: ${TARGETS}

run: all tcas.ll
	./markcond tcas.ll 2> tcas1.msg 1> tcas1.ll

tcas1.bc: run
	${CC} ${CFLAGS} -c -emit-llvm -g tcas1.ll

run-tcas: tcas1.bc
	LD_LIBRARY_PATH=${Z3_DIR} klee tcas1.bc

clean:
	rm -f ${TARGETS} *.bc *.ll

.SUFFIXES: .bc .ll

.c.bc:
	${CC} ${CFLAGS} -c -emit-llvm -g $< 

.bc.ll:
	llvm-dis $<
