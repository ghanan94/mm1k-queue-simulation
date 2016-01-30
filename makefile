CC = g++ -std=c++11
CFLAGS = -g -Wall
OBJECT_DIR = obj

.PHONY: default all clean

default: lab1
all: default

HEADERS = $(wildcard *.h)
#HEADERS = $(patsubst %.h, %.h, $(wildcard *.h))
#OBJECTS = $(patsubst %.cpp, $(OBJECT_DIR)/%.o, $(wildcard *.cpp))
LAB1_OBJECTS = $(OBJECT_DIR)/lab1.o $(OBJECT_DIR)/simulator.o
LAB1_DEBUG_OBJECTS = $(OBJECT_DIR)/lab1_debug.o $(OBJECT_DIR)/simulator.o
TEXT_EXPONENTIALRV_OBJECTS = $(OBJECT_DIR)/test_exponentialrv.o

$(OBJECT_DIR)/test_exponentialrv.o: test_exponentialrv.c
	gcc -g -Wall -lm -c $< -o $@

$(OBJECT_DIR)/%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

lab1: $(LAB1_OBJECTS)
	$(CC) $(CFLAGS) $(LAB1_OBJECTS) -o $@

lab1_debug: $(LAB1_DEBUG_OBJECTS)
	$(CC) $(CFLAGS) $(LAB1_DEBUG_OBJECTS) -o $@

test_exponentialrv: $(TEXT_EXPONENTIALRV_OBJECTS)
	gcc -g -Wall -lm $(TEXT_EXPONENTIALRV_OBJECTS) -o $@

clean:
	-rm -f $(OBJECT_DIR)/*.o
	-rm -f lab1
	-rm -f lab1_debug
	-rm -f test_exponentialrv
