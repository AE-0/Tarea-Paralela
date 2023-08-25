CFLAGS = -std=c++2b -O2
CFLAGS2 = -Wall -Wextra -Wpedantic -Wconversion -Werror -g
CXX = g++
MAKEFLAGS += --silent
FILENAME = main

default:
	$(CXX) $(CFLAGS) $(CFLAGS2) $(FILENAME).cpp -o $(FILENAME)
	./$(FILENAME) -i example.txt -o output.txt
run:
	$(CXX) $(CFLAGS) $(FILENAME).cpp -o $(FILENAME)
	./$(FILENAME) -i example.txt -o output.txt
	rm $(FILENAME)
