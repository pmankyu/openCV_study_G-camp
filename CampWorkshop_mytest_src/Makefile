CC = g++
CFLAGS = -g -Wall
SRCS = myopencv.cpp
PROG = myopencv

OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV)

.PHONY: all clean

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) contours.cpp number_recog.cpp $(LIBS)

all: $(PROG)

clean:
	rm -f $(OBJS) $(PROG)
