CXX_FLAGS = -std=c++2b -O2
CXX_FLAGS2 = -Wall -Wextra -Wpedantic -Wconversion -Werror -g
CXX_FLAGS3 = -std=c++2b -ggdb
CXX = g++
MAKEFLAGS += --silent
FILENAME = main

default:
	$(CXX) $(CXX_FLAGS) $(CXX_FLAGS2) $(FILENAME).cpp -o maze-solver
run:
	$(CXX) $(CXX_FLAGS) $(FILENAME).cpp -o $(FILENAME)
	./$(FILENAME) -i example.txt -o output.txt
	rm $(FILENAME)
debug:
	$(CXX) $(CXX_FLAGS3) $(FILENAME).cpp -o $(FILENAME)
clean:
	rm maze-solver output.txt
