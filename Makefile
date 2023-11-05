# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -static-libstdc++
SRC = dns.cpp config.cpp helpers.cpp

# Targets
all: dns

dns: main.cpp
	$(CXX) $(CXXFLAGS) -o $@ main.cpp $(SRC)

test: tests.sh
	./tests.sh

clean:
	rm -f dns

.PHONY: all clean tests
