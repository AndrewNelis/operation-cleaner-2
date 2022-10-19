CXX = g++
# SDL, zlib
# FLAGS = -Wall
FLAGS =-lSDL -lz -fpermissive -Wno-narrowing -Wno-write-strings
# libbuilding sources
SRCS=$(wildcard ./OC2/*.cpp)
# LIB_SRCS=$(wildcard ./OC2Building/*.cpp)

all: oc2

oc2:
	$(CXX) $(FLAGS) $(SRCS)

clean:
	rm -f *.out oc2building.so liboc2building.so OC2Building/*.o *.o
