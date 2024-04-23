# Name of executable
BINS = hw4 

# Setting the flags
CFLAGS = -g -Wall -Wextra -Wstrict-prototypes

# Link in support for debugging
LDFLAGS = -g

# Default target produced by entering "make" alone
.PHONY: default
default: $(BINS)

# Compile *.cpp into *.o
src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Remove *.o files, but leave executable
.PHONY: clean
clean:
	rm -f core* src/*.o *~

# Link *.o files into an executable
hw4: src/hw4.o
	$(CXX) $(CXXFLAGS) $^ -o $@

# Run tests
test: hw4
	./hw4

# Check for memory leaks
mem: hw4
	valgrind --leak-check=full --show-leak-kinds=all -s ./hw4

# Remove all files that can be reconstructed through "make"
.PHONY: immaculate
immaculate: clean
	rm -f $(BINS)
