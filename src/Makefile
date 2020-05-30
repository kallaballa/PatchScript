UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
  LIBS += -lstdc++fs
endif

ifndef STATIC
ifeq ($(UNAME_S), Darwin)
TARGET := libpatchscript.dylib
else
TARGET  := libpatchscript.so
endif
else
TARGET := libpatchscript.a
endif

ifdef JAVASCRIPT
TARGET := patchscript.html
endif

SRCS  := tonic_lua0.cpp tonic_lua1.cpp tonic_lua2.cpp PolySynth.cpp LV2Effect.cpp  PatchScript.cpp SqlStore.cpp FileStore.cpp PatFileReader.cpp PatFileWriter.cpp MetFileReader.cpp MetFileWriter.cpp RevFileReader.cpp RevFileWriter.cpp IOUtil.cpp SessionObject.cpp
CXXFLAGS += -I/usr/lib64/ -I../third/kaguya/include -I../third/lua-5.3.5/src -I../third/Tonic/src -I../third/SQLiteCPP -fpic
LIBS += -ltonic -llua -ldl -llilv-0 -lz -lsqlite3
LDFLAGS += -L../third/lua-5.3.5/src -L../third/Tonic/src -shared

#precompiled headers
HEADERS := tonic_lua.hpp 

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
ifndef STATIC
	${CXX} ${LDFLAGS} -o $@ $^ ${LIBS}
else
	ar -rcs $@ $^
endif

${OBJS}: %.o: %.cpp %.dep ${GCH}
	${CXX} ${CXXFLAGS} -o $@ -c $< 

${DEPS}: %.dep: %.cpp Makefile 
	${CXX} ${CXXFLAGS} -MM $< > $@ 

${GCH}: %.gch: ${HEADERS} 
	${CXX} ${CXXFLAGS} -o $@ -c ${@:.gch=.hpp}

install:
	mkdir -p ${DESTDIR}/${PREFIX}/bin
	cp ${TARGET} ${DESTDIR}/${PREFIX}/bin
	cp ../third/Tonic/src/libtonic.so ${DESTDIR}/${PREFIX}/lib

uninstall:
	rm ${DESTDIR}/${PREFIX}/bin/${TARGET}

clean:
	rm -f *~ ${DEPS} ${OBJS} ${GCH} ${TARGET}

distclean: clean



