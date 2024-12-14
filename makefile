CC     = gcc
OBJ    = main.o functions.o player.o hazard.o
TARGET = rong
FLAGS  = -lncurses

all: $(TARGET)
$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(FLAGS)

main.o: main.c headers.h
	$(CC) -c main.c

functions.o: functions.c headers.h	
	$(CC) -c functions.c

player.o: player.c headers.h	
	$(CC) -c player.c

hazard.o: hazard.c headers.h	
	$(CC) -c hazard.c

clean:
	rm -f $(OBJ) $(TARGET)