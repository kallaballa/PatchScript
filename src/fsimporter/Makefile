UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
  LIBS += -lstdc++fs
endif

ifndef STATIC
ifeq ($(UNAME_S), Darwin)
TARGET := fsimporter
else
TARGET  := fsimporter
endif
else
TARGET := fsimporter
endif

ifdef JAVASCRIPT
TARGET := fsimporter.html
endif

SRCS  :=  fsimporter.cpp
CXXFLAGS += -I/usr/lib64/ -I../../third/kaguya/include -I../../third/lua-5.3.5/src -I../../third/Tonic/src -I../../third/SQLiteCPP -I../
LIBS += -ltonic -llua -ldl -llilv-0 -lz -lsqlite3 -lpatchscript
LDFLAGS += -L../../third/lua-5.3.5/src -L../../third/Tonic/src -L../

#precompiled headers
HEADERS :=

GCH     := ${HEADERS:.hpp=.gch}
OBJS    := ${SRCS:.cpp=.o}
DEPS    := ${SRCS:.cpp=.dep} 

.PHONY: all release debug clean distclean

all: release
release: ${TARGET}
debug: ${TARGET}
info: ${TARGET}
profile: ${TARGET}
hardcore: ${TARGET}
asan: ${TARGET}

${TARGET}: ${OBJS}
	${CXX} ${LDFLAGS} -o $@ $^ ${LIBS}

${OBJS}: %.o: %.cpp %.dep ${GCH}
	${CXX} ${CXXFLAGS} -o $@ -c $< 

${DEPS}: %.dep: %.cpp Makefile 
	${CXX} ${CXXFLAGS} -MM $< > $@ 

${GCH}: %.gch: ${HEADERS} 
	${CXX} ${CXXFLAGS} -o $@ -c ${@:.gch=.hpp}

install:
	mkdir -p ${DESTDIR}/${PREFIX}/bin
	cp ${TARGET} ${DESTDIR}/${PREFIX}/bin

uninstall:
	rm ${DESTDIR}/${PREFIX}/bin/${TARGET}

clean:
	rm -f *~ ${DEPS} ${OBJS} ${GCH} ${TARGET}

distclean: clean



