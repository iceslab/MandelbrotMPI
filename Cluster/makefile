CXX=mpic++
CXXFLAGS=-c -O3 -std=c++11
LIBS=-lGL -lglut -lGLU -lgmpxx -lgmp
LDFLAGS=
SOURCES=main.cpp FractalCalc.cpp Display.cpp Utils.cpp Comunicator.cpp Master.cpp Slave.cpp
OBJECTS=$(SOURCES:%.cpp=%.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(EXECUTABLE) *.o
