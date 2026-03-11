CC      = g++
CCFLAGS = -O3
FLDEBUG = -O0 -g -Wall
# -Wno-unused-function

TARGET     = idiosync_parser
TARGET_DBG = main_debug
SRCS       = ./src/*.cpp
LIBS       = -licuuc -licudata -licui18n

install:
	$(CC) $(CCFLAGS) $(SRCS) $(LIBS) -o $(TARGET)

debug:
	$(CC) $(FLDEBUG) $(SRCS) $(LIBS) -o $(TARGET_DBG)

test:
	$(CC) $(FLDEBUG) $(SRCS) $(LIBS) -o $(TARGET_DBG) && ./$(TARGET_DBG)

clean:
	rm -rf *.o

uninstall:
	rm $(TARGET)

