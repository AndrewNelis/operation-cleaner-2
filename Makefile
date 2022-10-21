CXX = g++
FLAGS =-lSDL -lz -fpermissive -Wno-narrowing -Wno-write-strings  # -Wall
# libbuilding sources
SRCS=$(wildcard ./OC2/*.cpp)

all: oc2

oc2:
	$(CXX) $(FLAGS) $(SRCS) -o oc2

clean:
	rm -f *.out oc2building.so liboc2building.so OC2Building/*.o *.o oc2
