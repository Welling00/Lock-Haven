# Declaration of variables
CC = g++
CC_FLAGS = -w
CC_FLAGS += -g
CC_FLAGS += -std=c++11
 
# File names
EXEC = Server 
SOURCES = $(wildcard *.cpp)
SOURCES += $(wildcard pages/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
 
# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC)
 
# To obtain object files
%.o:%.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@
 
# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)