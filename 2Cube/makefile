OBJS = main.o MCTables.o Cores.o Dealer.o Task.o Simulate.o
HEADS = Dealer.h MCTables.h Cores.h  Utils.h Task.h Simulate.h
CFLAGS = -O4
CC = g++

TimeMarch:	$(OBJS)
		$(CC) $(CFLAGS) -o TimeMarch $(OBJS) -lm

main.o:		Task.h McTables.h Dealer.h Cores.h Simulate.h main.cpp
		$(CC) $(CFLAGS) -c main.cpp

Dealer.o:	Utils.h Cores.h Dealer.h  Dealer.cpp
		$(CC) $(CFLAGS) -c Dealer.cpp


MCTables.o:	Task.h Utils.h MCTables.h MCTables.cpp
		$(CC) $(CFLAGS) -c MCTables.cpp

Cores.o:	Cores.h Dealer.h Cores.cpp
		$(CC) $(CFLAGS) -c Cores.cpp

Simulate.o:	Task.h Dealer.h Cores.h Utils.h McTables.h Simulate.h Simulate.cpp
		$(CC) $(CFLAGS) -c Simulate.cpp

Task.o:		Task.h Dealer.h Task.cpp
		$(CC) $(CFLAGS) -c Task.cpp
