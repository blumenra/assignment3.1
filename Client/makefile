CFLAGS = -g -Wall -Weffc++ -std=c++11
CC = g++
BOOST  = -lboost_locale -lboost_system -lboost_filesystem -lboost_thread


# All Targets
all: client

# Executable "hello" depends on the files hello.o and run.o.
client: bin/TFTPclient.o  bin/connectionHandler.o  bin/BidiMessage.o  bin/bidiInputConverter.o  bin/BidiEncDec.o  bin/ClientProtocol.o
	@echo 'Building target: client'
	@echo 'Invoking: C++ Linker'
	$(CC) -o bin/TFTPclient  bin/TFTPclient.o  bin/connectionHandler.o  bin/BidiMessage.o  bin/bidiInputConverter.o  bin/BidiEncDec.o  bin/ClientProtocol.o  $(BOOST)
	@echo 'Finished building target: client'
	@echo '********************************** '
	@echo ' '

# Depends on the source and header files
bin/TFTPclient.o:   src/client.cpp
	$(CC) $(CFLAGS)  -c  -Iinclude  -o  bin/TFTPclient.o   src/client.cpp

bin/connectionHandler.o:   src/connectionHandler.cpp   include/connectionHandler.h
	$(CC) $(CFLAGS)  -c  -Iinclude  -o  bin/connectionHandler.o   src/connectionHandler.cpp

bin/BidiMessage.o:   src/BidiMessage.cpp   include/BidiMessage.h
	$(CC) $(CFLAGS)  -c  -Iinclude  -o  bin/BidiMessage.o   src/BidiMessage.cpp

bin/bidiInputConverter.o:   src/bidiInputConverter.cpp   include/bidiInputConverter.h
	$(CC) $(CFLAGS)  -c  -Iinclude  -o  bin/bidiInputConverter.o   src/bidiInputConverter.cpp

bin/BidiEncDec.o:   src/BidiEncDec.cpp   include/BidiEncDec.h
	$(CC) $(CFLAGS)  -c  -Iinclude  -o  bin/BidiEncDec.o   src/BidiEncDec.cpp

bin/ClientProtocol.o:   src/ClientProtocol.cpp   include/ClientProtocol.h
	$(CC) $(CFLAGS)  -c  -Iinclude  -o  bin/ClientProtocol.o   src/ClientProtocol.cpp


#Clean the build directory
clean: 
	rm -rf bin/*

