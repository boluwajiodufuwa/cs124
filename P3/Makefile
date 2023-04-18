# Makefile for Writing Make Files Example
 
# *****************************************************
# Variables to control Makefile operation
 
CC = g++ -std=c++17
CFLAGS = -O2 -Wall -Wextra

# ****************************************************
# Targets needed to bring the executable up to date

partition: partition.cpp
	$(CC) $(CFLAGS) partition.cpp -o partition -lm -lpthread

objects: partition.cpp tests.cpp experiment.cpp
	$(CC) $(CFLAGS) -c partition.cpp tests.cpp experiment.cpp

link: partition.o tests.o experiment.o
	$(CC) $(CFLAGS) -o partition partition.o tests.o experiment.o

tests:
	make objects
	make link
	./partition 1 0 0 