CC = g++
CC_FLAGS=-DLINUX -O2 -g
LINK=$(CC) -o $@  $<

FPGA_OCL_CFLAGS := $(shell aocl compile-config)
FPGA_OCL_LFLAGS := $(shell aocl link-config)

EXEC = test_contrast
SOURCES = main.cpp OpenCLUtils.cpp 

OBJECTS = $(SOURCES:.cpp=.o)

$(EXEC): $(OBJECTS)
        $(CC) $(OBJECTS) $(FPGA_OCL_LFLAGS) -o $(EXEC)

$(EXEC)_profile: $(OBJECTS)
        $(CC) -pg $(OBJECTS) -o $(EXEC)_gprof

# To obtain object files
%.o: %.cpp
        $(CC) -c $(CC_FLAGS) $(FPGA_OCL_CFLAGS) $< -o $@

all: $(EXEC)

profile: $(EXEC)_profile

clean:
        rm -f $(EXEC) $(OBJECTS)