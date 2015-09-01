CXXFLAGS =	-std=gnu++11 -O2 -g -Wall -fmessage-length=0 -Ilibs/

OBJS =		LuaBinder.o \
	libs/lua52/lapi.o \
	libs/lua52/lauxlib.o \
	libs/lua52/lbaselib.o \
	libs/lua52/lbitlib.o \
	libs/lua52/lcode.o \
	libs/lua52/lcorolib.o \
	libs/lua52/lctype.o \
	libs/lua52/ldblib.o \
	libs/lua52/ldebug.o \
	libs/lua52/ldo.o \
	libs/lua52/ldump.o \
	libs/lua52/lfunc.o \
	libs/lua52/lgc.o \
	libs/lua52/linit.o \
	libs/lua52/liolib.o \
	libs/lua52/llex.o \
	libs/lua52/lmathlib.o \
	libs/lua52/lmem.o \
	libs/lua52/loadlib.o \
	libs/lua52/lobject.o \
	libs/lua52/lopcodes.o \
	libs/lua52/loslib.o \
	libs/lua52/lparser.o \
	libs/lua52/lstate.o \
	libs/lua52/lstring.o \
	libs/lua52/lstrlib.o \
	libs/lua52/ltable.o \
	libs/lua52/ltablib.o \
	libs/lua52/ltm.o \
	libs/lua52/lundump.o \
	libs/lua52/lvm.o \
	libs/lua52/lzio.o

LIBS =

TARGET =	LuaBinder.exe

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
