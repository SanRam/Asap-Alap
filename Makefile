# I am a comment, and I want to say that the variable CC will be
# the compiler to use.
CC=g++
# Hey!, I am comment number 2. I want to say that CFLAGS will be the
# options I'll pass to the compiler.
CFLAGS=-c -Wall

all: asap-alap asap-alap.exe

asap-alap.exe : asap-alap.o
	$(CC) asap-alap.o -o asap-alap.exe
	
asap-alap: asap-alap.o
	$(CC) asap-alap.o -o asap-alap

asap-alap.o: asap-alap.cpp
	$(CC) $(CFLAGS) asap-alap.cpp

clean:
	rm -rf *o asap-alap.exe
	rm -rf *o asap-alap
