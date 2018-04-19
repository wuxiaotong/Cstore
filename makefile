CC=g++
#CFLAGS=-c -Wall -DTRACK -DORDERED
#CFLAGS=-c -Wall -DSINGLE -DORDERED
CFLAGS=-c -Wall -DORDERED
#CFLAGS=-c -Wall -DHASH

DBCONTROLLER=src/DBController.cpp
EXTERNSORT=src/externsort.cpp
MAIN=src/main.cpp

SOURCES=$(UTILS) $(IO) $(MAIN)
OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=db

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -pg -o $@
ifneq (,$(findstring HASH, $(CFLAGS)))
	+mkdir -p tmp
endif

.cpp.o:
	$(CC) $(CFLAGS) $< -pg -o $@

clean:
	rm src/*.o $(EXECUTABLE)
	rm -f bin/gmon.out
