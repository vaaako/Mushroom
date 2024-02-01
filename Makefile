CXX = clang++
CXXFLAGS = -std=c++20 -O3
DEVFLAGS =  -g -fdiagnostics-color=always -Wextra -Wall -Wuninitialized -Winit-self -Wfloat-equal -Wundef -Wpointer-arith -Wcast-align -Wstrict-overflow=5 -Wwrite-strings -Wcast-qual -Wswitch-default -Wconversion -Wunreachable-code

EDITLINE = $(wildcard includes/*.h)
LIBS     = -lstdc++ -lreadline

SRC_DIR = src
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp)

TARGET = mushroom # $@ (or func name)

$(TARGET): $(SRC_FILES)
	$(CXX) $(CXXFLAGS) $(SRC_FILES) $(LIBS) -o $@

dev: # Make with dev flags
	$(CXX) $(CXXFLAGS) $(DEVFLAGS) $(SRC_FILES) $(LIBS) -o $(TARGET)

install:
	make
	cp ./mushroom /usr/local/bin/mushroom

clean:
	rm -f $(TARGET)


# unset ASAN_SYMBOLIZER_PATH
# MSAN_SYMBOLIZER_PATH=$(which llvm-symbolizer-3.4) 
