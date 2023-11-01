# Makefile for a single C++ file

CC = g++
CFLAGS = -Wall -std=c++11
BOOST_INC = -I/opt/homebrew/Cellar/boost/1.83.0/include
BOOST_LIBS = -L/opt/homebrew/Cellar/boost/1.83.0/lib -lboost_program_options

simulation: simulation.cpp
	$(CC) $(CFLAGS) $(BOOST_INC) $< -o $@ $(BOOST_LIBS)

clean:
	rm -f simulation
