CC=gcc
CFLAGS=-Wall

final:Processor1.o Processor2.o Scheduler.o Generator.o MemRes_Manager.o OS.o

Processor1.o:Processor1.c
	$(CC) $(CFLAGS) -o Processor1.o Processor1.c

Processor2.o:Processor2.c
	$(CC) $(CFLAGS) -o Processor2.o Processor2.c

Scheduler.o:Scheduler.c
	$(CC) $(CFLAGS) -o Scheduler.o Scheduler.c

Generator.o:Generator.c
	$(CC) $(CFLAGS) -o Generator.o Generator.c

MemRes_Manager.o:MemRes_Manager.c
	$(CC) $(CFLAGS) -o MemRes_Manager.o MemRes_Manager.c

OS.o:OS.c
	$(CC) $(CFLAGS) -o OS.o OS.c
