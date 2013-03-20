OBJS = main.o MCTables.o Machine.o Simulate.o TCRAM.o
HEADS = MCTables.h Utils.h Machine.h Simulate.h TCRAM.h
CFLAGS = -O4
CC = c++ -std=c++0x 

Matrix:		$(OBJS)
		$(CC) $(CFLAGS) -o matmul  $(OBJS) -lm

main.o:		Machine.h Simulate.h MCTables.h TCRAM.h main.cpp
		$(CC) $(CFLAGS) -c main.cpp

Machine.o:	Machine.h Utils.h Machine.cpp
		$(CC) $(CFLAGS) -c Machine.cpp

MCTables.o:	Utils.h MCTables.h MCTables.cpp
		$(CC) $(CFLAGS) -c MCTables.cpp

TCRAM.o:	Utils.h MCTables.h TCRAM.cpp
		$(CC) $(CFLAGS) -c TCRAM.cpp

Simulate.o:	Simulate.h Utils.h MCTables.h Machine.h Simulate.cpp
		$(CC) $(CFLAGS) -c Simulate.cpp
