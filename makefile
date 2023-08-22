CFLAGS = -std=c++2b -O2
CFLAGS2 = -Wall -Wextra -Wpedantic -Wconversion -Werror -g
CXX = g++
MAKEFLAGS += --silent
FILENAME = main

default:
	$(CXX) $(CFLAGS) $(FILENAME).cpp -o $(FILENAME)
	./$(FILENAME)
