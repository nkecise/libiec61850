#
# build DynamicModelGenerator
#
PROJECT = modgen

# objects
OBJECTS	= DynamicModelGenerator.o pugixml.o SclParser.o AttributeType.o Utils.o

# cflags & lfalgs
CFLAGS = -I"pugixml"
LFLAGS = 

# target
all:$(PROJECT) 

# build rules
VPATH = ./pugixml

%.o:%.c
	$(CC) -c -g $(CFLAGS) $< -o $*.o

%.o:%.cpp
	$(CXX) -c -g $(CFLAGS) $< -o $*.o

%.o:%.cc
	$(CXX) -c -g $(CFLAGS) $< -o $*.o

$(PROJECT):$(OBJECTS)
	$(CXX) $(OBJECTS) -pthread -o $(PROJECT)

#SCLx.pb.cc:$(PROTO_ROOT)/SCLx.proto
#	protoc --cpp_out=. -I$(PROTO_ROOT) $(PROTO_ROOT)/SCLx.proto

clean:
	rm *.o
