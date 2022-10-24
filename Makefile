CXX = g++
FLAGS =-lSDL -lz -fpermissive -Wno-narrowing -Wno-write-strings  # -Wall
SRCS=$(wildcard ./OC2/*.cpp)
BSRCS=$(wildcard ./OC2Building/*.cpp)

all: oc2 oc2b

oc2b: $(BSRCS)
	$(CXX) $(FLAGS) $(BSRCS) -g -o oc2build

oc2: $(SRCS)
	$(CXX) $(FLAGS) $(SRCS) -g -o oc2

clean:
	rm -f oc2 oc2build
