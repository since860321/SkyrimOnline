CXX = g++

CXXFLAGS = -g -m32 -Wall -std=c++11

TARGET = GameServer

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:%.cpp=%.o)

$(TARGET) : $(OBJS)
	$(CXX) -g -m32 $(OBJS) -o $(TARGET)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean :
	rm $(OBJS)
	rm $(TARGET)
