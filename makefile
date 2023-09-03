CXX_FLAGS = -std=c++20 -O2
CXX_FLAGS2 = -Wall -Wextra -Wpedantic -Wconversion -Werror -g
CXX_FLAGS3 = -std=c++20 -ggdb
CXX = g++
MAKEFLAGS += --silent
FILENAME = maze-solver
SOURCES = main.cpp utils.cpp

default:
	$(CXX) $(CXX_FLAGS) $(CXX_FLAGS2) $(SOURCES) -o $(FILENAME)
run:
	$(CXX) $(CXX_FLAGS) $(SOURCES) -o $(FILENAME)
	./$(FILENAME) -i example.txt -o output.txt
	rm $(FILENAME)
	cat output.txt
debug:
	$(CXX) $(CXX_FLAGS3) $(SOURCES) -o $(FILENAME)
clean:
	rm $(FILENAME) output.txt
