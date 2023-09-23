# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall
SRC = dns.cpp config.cpp

# Targets
all: dns test

dns: main.cpp
	$(CXX) $(CXXFLAGS) -o $@ main.cpp $(SRC)

test: tests.cpp dns.cpp config.cpp
	$(CXX) $(CXXFLAGS) -o $@ tests.cpp $(SRC)

clean:
	rm -f dns test

.PHONY: all clean tests
