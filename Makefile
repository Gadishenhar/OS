# Makefile for the smash program
CC = g++
CPPFLAGS = -std=c++11 -Wall -Werror -pedantic-errors -DNDEBUG -pthread
CCLINK = $(CC)
OBJS = bank.o atm.o bank_data.o
RM = rm -f
TARGET = Bank
# Creating the  executable
Bank: $(OBJS)
	$(CCLINK) -o Bank $(OBJS)
# Creating the object files
bank.o: bank.cpp atm.h bank_data.h 
atm.o: atm.cpp atm.h bank_data.h
bank_data.o : bank_data.cpp bank_data.h
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*
	

