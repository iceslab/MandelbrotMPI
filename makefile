CXX=mpic++
CXXFLAGS=-c -O3 -std=c++11
LIBS=-lGL -lglut -lGLU
LDFLAGS=
SOURCES=MandelbrotSet.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(EXECUTABLE) *.o