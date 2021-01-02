CC = g++
CXXFLAGS =  -std=c++11 -Wall -Werror -pedantic-errors -DNDEBUG 
CCLINK = $(CC)
OBJS = bank_data.o atm.o bank.o 
RM = rm -f
TARGET = Bank
# Creating the  executable
$(TARGET): $(OBJS)
	$(CCLINK) -o $(TARGET) $(OBJS) -pthread  
# Creating the object files
bank_data.o: bank_data.cpp bank_data.h
atm.o: atm.cpp atm.h
bank.o: bank.cpp  atm.h bank_data.h
# Cleaning old files before new make
.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) *.o *~ "#"* core.* *.gch
