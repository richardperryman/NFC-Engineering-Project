# Compiler stuff
CC = g++
DEBUG = -DDEBUG
INCLUDE = ./include
LFLAGS = -Wall -I$(INCLUDE) -std=c++11
CFLAGS = $(LFLAGS) -c

# File stuff
EXECUTABLE = GPIOTest
OBJECTS = main.o DecodedPacket.o EncodedPacket.o GPIOPin.o Serial.o Debug.o
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
main.o : main.cpp $(INCLUDE)/GPIOPin.h $(INCLUDE)/DecodedPacket.h $(INCLUDE)/EncodedPacket.h  $(INCLUDE)/Serial.h $(INCLUDE)/Types.h $(INCLUDE)/Debug.h
	$(CC) $(CFLAGS) main.cpp
	
DecodedPacket.o : $(INCLUDE)/DecodedPacket.h DecodedPacket.cpp $(INCLUDE)/Types.h
	$(CC) $(CFLAGS) DecodedPacket.cpp
	
EncodedPacket.o : $(INCLUDE)/EncodedPacket.h EncodedPacket.cpp $(INCLUDE)/Types.h
	$(CC) $(CFLAGS) EncodedPacket.cpp

GPIOPin.o : $(INCLUDE)/GPIOPin.h GPIOPin.cpp $(INCLUDE)/Debug.h $(INCLUDE)/Types.h
	$(CC) $(CFLAGS) GPIOPin.cpp

Serial.o : $(INCLUDE)/Serial.h $(INCLUDE)/Debug.h $(INCLUDE)/Types.h
	$(CC) $(CFLAGS) Serial.cpp

Debug.o : $(INCLUDE)/Debug.h Debug.cpp
	$(CC) $(CFLAGS) Debug.cpp
