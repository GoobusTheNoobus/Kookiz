CXX = g++
CXXFLAGS = -O3 -march=native -Isrc -MMD -MP

SRC = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp)
OBJ = $(SRC:.cpp=.o)
DEP = $(OBJ:.o=.d)

TARGET = kookiz

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

clean:
	rm -f $(OBJ) $(DEP) $(TARGET)

-include $(DEP)
