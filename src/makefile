# Declaration of variables
CC = g++
FLAGS = -std=c++11
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# File names
EXEC = gbemu
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(LIBS)

# To obtain object files
%.o: %.cpp
	$(CC) -c $(FLAGS) $< -o $@

# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)
