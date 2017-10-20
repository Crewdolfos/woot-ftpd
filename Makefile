CC=gcc
CFLAGS=-c -Wall -g -O0 -fno-stack-protector -z execstack -no-pie
LDFLAGS=-lm
SOURCES=server.c handles.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=wootftpd

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean: 
	rm -rf *.o wootftpd
