#---g++ -c lrucache.cc -llog4cpp -lboost -std=c++11

CC=g++
CFLAGS= -g -Wall -D_REENTRANT -D_GNU_SOURCE -fPIC -std=c++11
SOFLAGS = -shared
LDFLAGS= -lpthread -lboost -llog4cpp 

TARGET=lrucache.so
TESTR=testr

#----------------------------------------
LIBOBJS=lrucache.o

#----------------------------------------
all: $(TARGET) $(TESTR)

$(TARGET): $(LIBOBJS)
	$(CC) $(SOFLAGS) $^ -o $@
	
$(TESTR): 


clean:
	rm -rf $(TARGET) $(LIBOBJS)

#----------------------------------------
# make rule

%.o : %.cpp
	$(CC) $(CFLAGS) -c $^ -o $@
	
%.o : %.cc
	$(CC) $(CFLAGS) -c $^ -o $@	