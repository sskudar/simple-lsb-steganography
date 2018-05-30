#glavni program
PROJECT = stegano 

#svi source fajlovi
SOURCE = stegano.cpp

#svi headeri
HEADERS = steg_header.h
CC = g++  	#... ako zelimo kompajler gcc; default je cc
CFLAGS = -Wall -g
LDFLAGS =
OBJECTS = ${SOURCE:.c=.o}
LOCATION = -L/usr/lib -lbsd -lhdf5_cpp -lgtkglext-x11-1.0

$(PROJECT): $(OBJECTS)
		$(CC) `pkg-config --cflags --libs opencv` $(CFLAGS) $(OBJECTS) -o $(PROJECT) $(LOCATION)

$(OBJECTS): $(HEADERS)

clean:
	-rm -f $(PROJECT) *.o *.core
