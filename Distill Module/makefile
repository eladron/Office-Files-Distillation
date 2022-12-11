# Makefile for Writing Make Files Example
 
# *****************************************************
# Variables to control Makefile operation
 
CC = g++
CFLAGS = -Wall -g
 
# ****************************************************
# Targets needed to bring the executable up to date
 
main: distill.o Distilator.o
	$(CC) $(CFLAGS) -o distill distill.o Distilator.o
 
# The main.o target can be written more simply
 
distill.o: distill.cpp Distilator.hpp
	$(CC) $(CFLAGS) -c distill.cpp
 
Distilator.o: Distilator.hpp Tags.h

clean:
	rm distill *.o
 
