CXX = g++
FLAGS =-lSDL -lz -fpermissive -Wno-narrowing -Wno-write-strings  # -Wall
SRCS=$(wildcard ./src/OC2/*.cpp)
BSRCS=$(wildcard ./src/OC2Building/*.cpp)
OC2=./oc2/oc2
OC2BUILD=./oc2/oc2build

all: $(OC2) $(OC2BUILD)

$(OC2BUILD): $(BSRCS)
	$(CXX) $(FLAGS) $(BSRCS) -g -o $(OC2BUILD)

$(OC2): $(SRCS)
	$(CXX) $(FLAGS) $(SRCS) -g -o $(OC2)

clean:
	rm -f $(OC2) $(OC2BUILD)
