CXX = g++
CXXFLAGS = -std=c++17 -Wall

LIBS = -lcryptopp -lboost_program_options

SRCS = main.cpp Vector.cpp Logger.cpp HashProcessor.cpp Server.cpp Communicator.cpp
TARGET = server

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)

