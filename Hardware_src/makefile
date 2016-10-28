# Compiler stuff
CC = g++
DEBUG = -DDEBUG
INCLUDE = ./include
LFLAGS = -Wall -I$(INCLUDE)
CFLAGS = $(LFLAGS) -c

# File stuff
EXECUTABLE = GPIOTest
OBJECTS = main.o GPIOPin.o Serial.o Debug.o
RM = rm -f *.o

default : $(EXECUTABLE)

debug : CFLAGS += $(DEBUG)
debug : $(EXECUTABLE)

clean :
	$(RM) *~ $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $(EXECUTABLE)
	$(RM)

# Intermediate object files
main.o : main.cpp $(INCLUDE)/GPIOPin.h $(INCLUDE)/Serial.h
	$(CC) $(CFLAGS) main.cpp

GPIOPin.o : $(INCLUDE)/GPIOPin.h GPIOPin.cpp $(INCLUDE)/Debug.h
	$(CC) $(CFLAGS) GPIOPin.cpp

Serial.o : $(INCLUDE)/Serial.h $(INCLUDE)/Debug.h
	$(CC) $(CFLAGS) Serial.cpp

Debug.o : $(INCLUDE)/Debug.h Debug.cpp
	$(CC) $(CFLAGS) Debug.cpp
