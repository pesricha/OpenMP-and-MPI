CXX = g++
CXXFLAGS = -std=c++11 
TARGET = prefix_sum
SRCS = prefix_sum.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)