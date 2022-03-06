# flags, default optimization level
CLFLAGS=-Wall -Og
# set up for use in gdb
DFLAGS=-g
#additional librarries
LDFLAGS=-lm -pthread
PROGS= prodcon1 prodcon2 prodcon3

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $(DFLAGS) $< $(LDFLAGS) -o $@

clean:
	rm -f $(PROGS) *.o ~* a.out
