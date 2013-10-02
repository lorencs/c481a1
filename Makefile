CXXFLAGS =	-O2 -g -fmessage-length=0

OBJS =		c481a1.o

LIBS = 		-pthread

TARGET =	c481a1

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
