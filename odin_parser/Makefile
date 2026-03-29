FLAGS   = -o:speed
# FLDEBUG = 

TARGET     = ./idiosync_parser_odin
# TARGET_DBG = ./idiosync_parser_odin_debug
SRC        = ./src

build:
	odin build $(SRC) $(FLAGS) -out:$(TARGET)

run:
	odin build $(SRC) $(FLAGS) -out:$(TARGET) && $(TARGET)
