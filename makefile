#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: server client

# Tool invocations
# Executable "ass1" depends on the files server.o client.o 
server: server.o
	gcc -o server server.c

client: client.o
	gcc -o client client.c

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o server client 
