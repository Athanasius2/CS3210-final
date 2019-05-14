CC = clang++
CFLAGS = -std=c++17 -g -o
OUT = final
CFILES = main.cpp Actor.cpp Animal.cpp Plant.cpp Environment.cpp

all:
	$(CC) $(CFLAGS) $(OUT) $(CFILES)