# Copyright 2017 National University of Singapore
#
# See LICENSE.md for license information.

CC=clang-3.4
CXX=clang++-3.4
CXXFLAGS=`llvm-config-3.4 --cxxflags` -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0
LDFLAGS=`llvm-config-3.4 --ldflags`
LDLIBS=`llvm-config-3.4 --libs` -lpthread -lncurses -ldl
EXAMPLES=tritype.bc

TARGETS=markcond ${EXAMPLES}

all: ${TARGETS}

clean:
	rm -f ${TARGETS} *.bc *.ll

.SUFFIXES: .bc .ll

.c.bc:
	${CC} ${CFLAGS} -c -emit-llvm -g $<

.bc.ll:
	llvm-dis $<
