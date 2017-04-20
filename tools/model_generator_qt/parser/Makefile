#
# build DynamicModelGenerator
#
PROJECT = modgen

# objects
LIB_OBJECTS	= DynamicModelGenerator.o pugixml.o SclParser.o AttributeType.o Utils.o
APP_OBJECTS	= test.o

# cflags & lfalgs
CFLAGS = -I"pugixml" -Wall
LFLAGS = 

# target
all:$(PROJECT)

# build rules
VPATH = ./pugixml

%.o:%.c
	$(CC) -c -g -fPIC $(CFLAGS) $< -o $*.o

%.o:%.cpp
	$(CXX) -c -g -fPIC $(CFLAGS) $< -o $*.o

%.o:%.cc
	$(CXX) -c -g -fPIC $(CFLAGS) $< -o $*.o

$(PROJECT):$(LIB_OBJECTS)
	$(CXX) $(LIB_OBJECTS) -pthread -o $(PROJECT)

test:$(APP_OBJECTS)
	$(CXX) $(APP_OBJECTS) -o test

#SCLx.pb.cc:$(PROTO_ROOT)/SCLx.proto
#	protoc --cpp_out=. -I$(PROTO_ROOT) $(PROTO_ROOT)/SCLx.proto

clean:
	rm *.o
