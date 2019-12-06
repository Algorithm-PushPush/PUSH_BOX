CC = g++
CPPFLAGS = -W -Wall
TARGET = PUSH
SOURCES = main.cpp\
	  Push_BOX.cpp\
	  sokoban.cpp

LDPFLAGS= -lncurses

all:
	$(CC) $(SOURCES) -o $(TARGET) $(LDPFLAGS) $(CPPFLAGS)
clean:
	rm -rk *.o $(TARGET)



