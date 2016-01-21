TARGET = lab1
LIBS = -lm
CC = g++
CFLAGS = -g -Wall
OBJECT_DIR = obj

.PHONY: default all clean

default: $(TARGET)
all: default

HEADERS = $(wildcard *.h)
#HEADERS = $(patsubst %.h, %.h, $(wildcard *.h))
OBJECTS = $(patsubst %.cpp, $(OBJECT_DIR)/%.o, $(wildcard *.cpp))

$(OBJECT_DIR)/%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f $(OBJECT_DIR)/*.o
	-rm -f $(TARGET)
