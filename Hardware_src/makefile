# Compiler stuff
CC = g++
#CC = arm-linux-gnueabi-g++
DEBUG = -DDEBUG
INCLUDE = ./include
LDFLAGS = $(shell pkg-config --libs libcurl) $(shell pkg-config --libs json-c)
CFLAGS = -Wall -I$(INCLUDE) -std=c++11 -c -g

# File stuff
EXECUTABLE = GPIOTest
OBJECTS = main.o Packet.o DecodedPacket.o EncodedPacket.o AuthenticationToken.o GPIOPin.o Serial.o ServerConnection.o AuthenticationModule.o Debug.o
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
main.o : main.cpp $(INCLUDE)/GPIOPin.h $(INCLUDE)/DecodedPacket.h $(INCLUDE)/EncodedPacket.h  $(INCLUDE)/Serial.h $(INCLUDE)/Types.h $(INCLUDE)/AuthenticationModule.h $(INCLUDE)/Debug.h
	$(CC) $(CFLAGS) main.cpp $(LDFLAGS)
	
Packet.o: Packet.cpp $(INCLUDE)/Packet.h $(INCLUDE)/Types.h
	$(CC) $(CFLAGS) Packet.cpp
	
DecodedPacket.o : $(INCLUDE)/DecodedPacket.h DecodedPacket.cpp $(INCLUDE)/Types.h
	$(CC) $(CFLAGS) DecodedPacket.cpp
	
EncodedPacket.o : $(INCLUDE)/EncodedPacket.h EncodedPacket.cpp $(INCLUDE)/Types.h $(INCLUDE)/Packet.h
	$(CC) $(CFLAGS) EncodedPacket.cpp
	
AuthenticationToken.o : $(INCLUDE)/AuthenticationToken.h AuthenticationToken.cpp $(INCLUDE)/Types.h
	$(CC) $(CFLAGS) AuthenticationToken.cpp
	
AuthenticationModule.o: $(INCLUDE)/AuthenticationModule.h AuthenticationModule.cpp $(INCLUDE)/Serial.h $(INCLUDE)/AuthenticationToken.h
	$(CC) $(CFLAGS) AuthenticationModule.cpp

GPIOPin.o : $(INCLUDE)/GPIOPin.h GPIOPin.cpp $(INCLUDE)/Debug.h $(INCLUDE)/Types.h
	$(CC) $(CFLAGS) GPIOPin.cpp

Serial.o : $(INCLUDE)/Serial.h $(INCLUDE)/Debug.h $(INCLUDE)/Types.h
	$(CC) $(CFLAGS) Serial.cpp
	
ServerConnection.o: $(INCLUDE)/ServerConnection.h ServerConnection.cpp $(INCLUDE)/Types.h $(INCLUDE)/AuthenticationToken.h $(INCLUDE)/AuthenticationModule.h
	$(CC) $(CFLAGS) ServerConnection.cpp $(LDFLAGS) 

Debug.o : $(INCLUDE)/Debug.h Debug.cpp
	$(CC) $(CFLAGS) Debug.cpp
