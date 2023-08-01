CXX := g++
CXXFLAGS := -std=c++11
OBJECTS := trace.o vec3.o main.o
TARGET := main

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

clean:
	rm *.o main

