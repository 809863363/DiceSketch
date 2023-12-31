EXEC += persistence
all: $(EXEC)

CFLAGS = -Wall -std=c++11 -O3
HEADER += hash.h datatypes.hpp util.h adaptor.hpp 
SRC += hash.c adaptor.cpp
SKETCHHEADER += dicesketch.hpp
SKETCHSRC += dicesketch.cpp
LIBS= -lpcap 

persistence: persistence.cpp $(SRC) $(HEADER) $(SKETCHHEADER) 
	g++ $(CFLAGS) $(INCLUDES) -o $@ $< $(SRC) $(SKETCHSRC) $(LIBS) 


clean:
	rm -rf $(EXEC)
	rm -rf *log*
	rm -rf *out*
