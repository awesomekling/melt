BINARY = melt

OBJS = \
	Node.o \
	Element.o \
        Document.o \
        Text.o \
        ParentNode.o \
        Dump.o \
        Parser.o \
        test.o

CXXFLAGS = -Os -std=c++1z -W -Wall -g -I../AK
LDFLAGS =
LIBS =

$(BINARY): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

all: $(BINARY)

clean:
	rm -f $(BINARY) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
