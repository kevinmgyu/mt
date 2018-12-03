SOURCE  := $(wildcard *.c)
SOURCE  += $(wildcard *.cpp)
OBJS    := $(patsubst %.cpp, %.o, $(SOURCE))
# CC      := g++-4.8
CC		:= g++
CFLAGS  := -O2 -D__TEST_ENV__ -D__STDC_FORMAT_MACROS -fpermissive -Wall -Wnarrowing -Wno-strict-aliasing -Wno-uninitialized  -g -rdynamic -std=c++11
//CFLAGS  := -D__STDC_FORMAT_MACROS -Wall -Wnarrowing -Wno-strict-aliasing -Wno-uninitialized  -g -rdynamic -std=c++11
LFLAGS  :=  -g -rdynamic

.PHONY : all clean install 

all : $(TARGET)

clean :
	rm -rf *.d *.o $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(LFLAGS) -o $@ $(OBJS) $(LIBS)
%.o:%.cpp 
	$(CC) $(CFLAGS) -c $< -o $@ $(INCS)

install :

