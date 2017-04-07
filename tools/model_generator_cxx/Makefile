#
# build DynamicModelGenerator
#
PROJECT = genconfig

# objects
LIB_OBJECTS	= DynamicModelGenerator.o pugixml.o SclParser.o AttributeType.o Utils.o
APP_OBJECTS	= test.o

# cflags & lfalgs
CFLAGS = -I"pugixml"
LFLAGS = 

# target
all:lib$(PROJECT).so test

# build rules
VPATH = ./pugixml

%.o:%.c
	$(CC) -c -g -fPIC $(CFLAGS) $< -o $*.o

%.o:%.cpp
	$(CXX) -c -g -fPIC $(CFLAGS) $< -o $*.o

%.o:%.cc
	$(CXX) -c -g -fPIC $(CFLAGS) $< -o $*.o

lib$(PROJECT).so:$(LIB_OBJECTS)
	$(CXX) $(LIB_OBJECTS) -shared -pthread -o lib$(PROJECT).so

test:$(APP_OBJECTS)
	$(CXX) $(APP_OBJECTS) -o test

#SCLx.pb.cc:$(PROTO_ROOT)/SCLx.proto
#	protoc --cpp_out=. -I$(PROTO_ROOT) $(PROTO_ROOT)/SCLx.proto

clean:
	rm *.o
