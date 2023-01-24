# Makefile for Writing Make Files Example
 
# *****************************************************
# Variables to control Makefile operation
 
CC = g++
CFLAGS = -Wall -g -std=c++17
 
# ****************************************************
# Targets needed to bring the executable up to date
 
main: distill.o Distilator.o minidocx.o pugixml.o zip.o
	$(CC) $(CFLAGS) -o distill distill.o Distilator.o pugixml.o minidocx.o zip.o
 
# The main.o target can be written more simply
distill.o: distill.cpp
	$(CC) $(CFLAGS) -c distill.cpp
 
Distilator.o: Distilator.hpp Distilator.cpp
	$(CC) $(CFLAGS) -c Distilator.cpp

minidocx.o: minidocx.hpp minidocx.cpp
	$(CC) $(CFLAGS) -c minidocx.cpp

pugixml.o: pugixml.hpp pugixml.cpp
	$(CC) $(CFLAGS) -c pugixml.cpp

zip.o: zip.h zip.c
	$(CC) $(CFLAGS) -c zip.c