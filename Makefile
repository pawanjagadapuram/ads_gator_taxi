 # Compiler Settings
CC=g++
CFLAGS=-std=c++11

# Targets
default: clean run

run:
	$(CC) $(CFLAGS) -o gatorTaxi main.cpp

.PHONY: clean
clean:
	rm -rf gatorTaxi output*