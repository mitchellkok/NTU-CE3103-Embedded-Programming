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
ALL_MULTI = $(MULTI_O) $(MULTI)

FIFO_R = fifo_read
FIFO_W = fifo_write
FIFO_PIPE = mypipe
ALL_FIFO = $(FIFO_R) $(FIFO_W)

SOCKET_C = socket_client
SOCKET_S = socket_server
ALL_SOCKET = $(SOCKET_C) $(SOCKET_S)

MQUEUE = mqueue
MQUEUE_O = mqueue.o
MQUEUE_SRC = mqueue.c
ALL_MQUEUE = $(MQUEUE_O) $(MQUEUE)

GPIO = gpio_basic gpio_lkm
GPIO_SRC = gpio_basic.c gpio_lkm.c

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

# MESSAGE QUEUE
$(MQUEUE): $(MQUEUE_O)
	$(CC) -o $@ $^ -lrt $(THREAD_LIB)

$(MQUEUE_O): $(MQUEUE_SRC)
	$(CC) -c -o $@ $<

# GPIO - Compile on RPI
#$(GPIO): $(GPIO_SRC)
#	$(CC) -o $@ $<

# GPIO LKM - Compile on RPI (gives .ko file)
#The 1st line defines the loadable module to be built (known as goal definition)
#obj-m := gpio_lkm.o
#all:
	#make -C /usr/src/linux-headers-`uname -r` M=`pwd` modules

# Clean
clean:
	sudo rm -f $(ALL_SSMULTI) $(ALL_FIFO) $(FIFO_PIPE) $(ALL_SOCKET) $(ALL_MQUEUE)