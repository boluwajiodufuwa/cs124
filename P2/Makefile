# Makefile for Writing Make Files Example
 
# *****************************************************
# Variables to control Makefile operation
 
CC = g++ -std=c++17
CFLAGS = -O2 -Wall -Wextra

# ****************************************************
# Targets needed to bring the executable up to date

strassen: strassen.cpp
	$(CC) $(CFLAGS) strassen.cpp -o strassen -lm -lpthread

objects: strassen.cpp tests.cpp
	$(CC) $(CFLAGS) -c strassen.cpp tests.cpp

link: strassen.o tests.o
	$(CC) $(CFLAGS) -o strassen strassen.o tests.o

tests:
	make objects
	make link
	./strassen 1 0 0 