CXX=mpic++
CXXFLAGS=-c -O3 -std=c++11 -ggdb1 -I /usr/include/mysql -static-libgcc
LIBS= -lmysqlclient -static-libgcc
LDFLAGS=
SOURCES=main.cpp FractalCalc.cpp Utils.cpp Master.cpp Slave.cpp ../NetCpp/MysqlComm.cpp
OBJECTS=$(SOURCES:%.cpp=%.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(EXECUTABLE) *.o
