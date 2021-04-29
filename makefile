# MAKEFILE
#	- Files that contain instructions for compilation of C programs
#	- Must be named Makefile or makefile
#	- Use with 'make' commmand in terminal
#	- 'make' without arguments processes the FIRST rule in the makefile

#	- Rules are sensitive to tabs and whitespaces
#	- Rules take the form: 
#		$(TARGET): $(DEPENDENCIES)
#			$(INSTRUCTION)

#	- Can declare variable names and refer to them with $(name)
#	- Only need whitespace to separate multiple files under one object
#	- Other symbols:
#		$@ = TARGET --> Item before colon
#		$^ = ALL DEPENDENCIES --> All items after colon
#		$< = LEFT-MOST DEPENDENCY

# GCC Arguments
#	-c	--> Compile without linking (gives .o)
#	-g	--> Compile with debug info (GDB name, run)
#	-pg	--> Compile with profiler tools (.\name, gprof gmon.out > gmon.txt)

# Objects
CC = gcc
THREAD_LIB = -lpthread

MULTI = multi
MULTI_O = multi.o
MULTI_SRC = multi.c

FIFO_R = fifo_read
FIFO_W = fifo_write
FIFO_PIPE = mypipe
ALL_FIFO = $(FIFO_R) $(FIFO_W)

SOCKET_C = socket_client
SOCKET_S = socket_server
ALL_SOCKET = $(SOCKET_C) $(SOCKET_S)

# Rules
all: $(MULTI) $(ALL_FIFO) $(ALL_SOCKET)

# MULTI
$(MULTI): $(MULTI_O)
	$(CC) -o $@ $< $(THREAD_LIB)

$(MULTI_O): $(MULTI_SRC)
	$(CC) -c -o $@ $<

# FIFO
$(FIFO_R): fifo_read.c
	$(CC) -o $@ $^

$(FIFO_W): fifo_write.c
	$(CC) -o $@ $^

# SOCKET
$(SOCKET_C): socket_client.c
	$(CC) -o $@ $^

$(SOCKET_S): socket_server.c
	$(CC) -o $@ $^ $(THREAD_LIB)

# Clean
clean:
	sudo rm -f $(MULTI) $(ALL_FIFO) $(FIFO_PIPE) $(ALL_SOCKET)