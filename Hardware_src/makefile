# Compiler stuff
CC = g++
DEBUG = -DDEBUG
INCLUDE = ./include
LDFLAGS = $(shell pkg-config --libs libcurl) $(shell pkg-config --libs json-c)
CFLAGS = -Wall -I$(INCLUDE) -std=c++11 -c

# File stuff
EXECUTABLE = GPIOTest
OBJECTS = main.o DecodedPacket.o EncodedPacket.o GPIOPin.o Serial.o ServerConnection.o Debug.o
RM = rm -f *.o

default : $(EXECUTABLE) 

debug : CFLAGS += $(DEBUG)
debug : $(EXECUTABLE)

clean :
	$(RM) *~ $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(LDFLAGS)
	$(RM)

# Intermediate object files
main.o : main.cpp $(INCLUDE)/GPIOPin.h $(INCLUDE)/DecodedPacket.h $(INCLUDE)/EncodedPacket.h  $(INCLUDE)/Serial.h $(INCLUDE)/Types.h $(INCLUDE)/Debug.h
	$(CC) $(CFLAGS) main.cpp $(LDFLAGS)
	
DecodedPacket.o : $(INCLUDE)/DecodedPacket.h DecodedPacket.cpp $(INCLUDE)/Types.h
	$(CC) $(CFLAGS) DecodedPacket.cpp $(LDFLAGS)
	
EncodedPacket.o : $(INCLUDE)/EncodedPacket.h EncodedPacket.cpp $(INCLUDE)/Types.h
	$(CC) $(CFLAGS) EncodedPacket.cpp $(LDFLAGS)

GPIOPin.o : $(INCLUDE)/GPIOPin.h GPIOPin.cpp $(INCLUDE)/Debug.h $(INCLUDE)/Types.h
	$(CC) $(CFLAGS) GPIOPin.cpp $(LDFLAGS)

Serial.o : $(INCLUDE)/Serial.h $(INCLUDE)/Debug.h $(INCLUDE)/Types.h
	$(CC) $(CFLAGS) Serial.cpp $(LDFLAGS)
	
ServerConnection.o: $(INCLUDE)/ServerConnection.h ServerConnection.cpp $(INCLUDE)/Types.h
	$(CC) $(CFLAGS) ServerConnection.cpp $(LDFLAGS) 

Debug.o : $(INCLUDE)/Debug.h Debug.cpp
	$(CC) $(CFLAGS) Debug.cpp
