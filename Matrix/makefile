OBJS = main.o MCTables.o Cores.o Dealer.o Task.o  Machine.o Simulate.o WriteCore.o
HEADS = Dealer.h MCTables.h Cores.h  Utils.h Task.h  Machine.h Simulate.h WriteCore.h
CFLAGS = -O4
CC = c++ -std=c++0x

Matrix:		$(OBJS)
		$(CC) $(CFLAGS) -o Matrix  $(OBJS) -lm

main.o:		Task.h MCTables.h Dealer.h Cores.h WriteCore.h  main.cpp
		$(CC) $(CFLAGS) -c main.cpp

Dealer.o:	Utils.h Cores.h Dealer.h  Dealer.cpp
		$(CC) $(CFLAGS) -c Dealer.cpp

Machine.o:	Machine.h Machine.cpp
		$(CC) $(CFLAGS) -c Machine.cpp

MCTables.o:	Task.h Utils.h MCTables.h MCTables.cpp
		$(CC) $(CFLAGS) -c MCTables.cpp

Cores.o:	Cores.h Dealer.h Cores.cpp
		$(CC) $(CFLAGS) -c Cores.cpp

Task.o:		Task.h Dealer.h Task.cpp
		$(CC) $(CFLAGS) -c Task.cpp

Simulate.o:	Simulate.h Task.h Utils.h Simulate.cpp
		$(CC) $(CFLAGS) -c Simulate.cpp

WriteCore.o:    WriteCore.h WriteCore.cpp
		$(CC) $(CFLAGS) -c WriteCore.cpp

