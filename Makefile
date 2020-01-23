all: Emulator

ifdef EMULATE
SERVARCH=
CXXFLAGS += -DEMU=1
else
#SERVARCH=arm-linux-gnueabihf-
#SERVARCH=i486-openwrt-linux-

#CXXFLAGS += -DARM=1
endif

CXX=$(SERVARCH)g++

OBJ=Emulator.o Thread.o ./Socket/Socket.o CCmdInterpreter.o \
	./Socket/ServerSocket.o 
	

Emulator: $(OBJ)
	$(CXX) $(OBJ) -Wall -lpthread -lrt -o  Emulator

install:
	sudo cp Emulator ~/TI_SDK/targetNFS/usr/sbin
	sudo chown root:root ~/TI_SDK/targetNFS/usr/sbin/Emulator


clean:
	rm -rf *.o core Emulator  ./Socket/*.o ./revision.h
