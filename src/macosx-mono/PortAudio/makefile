
COM  = pa_common
CFLAGS := -O3 -I$(COM)
OBJ  = $(COM)/pa_lib.o pa_mac_core.o
TARGET = ../libPortAudio.o

all : $(TARGET)

$(TARGET) : $(OBJ)
	ld -r $(OBJ) -o $(TARGET)

clean :
	rm $(OBJ) $(TARGET)
