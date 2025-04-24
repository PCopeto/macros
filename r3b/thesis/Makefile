CXX=g++
LINKER=g++

ROOTCONFIG := root-config

CFLAGS := $(shell $(ROOTCONFIG) --cflags)
CFLAGS += -I${FAIRROOTPATH}/include
CFLAGS += -I$(SIMPATH)/include
CFLAGS += -I$(ROOT_INCLUDE_PATH)
CFLAGS += -I$(ROOT_INCLUDE_DIR)
CFLAGS += -I$(VMCWORKDIR)
CFLAGS += -I$(VMCWORKDIR)/tracking
CFLAGS += -I$(VMCWORKDIR)/field
CFLAGS += -I$(VMCWORKDIR)/r3bdata
CFLAGS += -I$(VMCWORKDIR)/r3bdata/rpcData
CFLAGS += -I$(VMCWORKDIR)/r3bdata/losData
CFLAGS += -I$(VMCWORKDIR)/r3bdata/frsData
CFLAGS += -I$(VMCWORKDIR)/r3bdata/trackerData
CFLAGS += -I$(VMCWORKDIR)/r3bdata/losData
CFLAGS += -I$(VMCWORKDIR)/r3bdata/tofData
CFLAGS += -I$(VMCWORKDIR)/r3bdata/fibData
CFLAGS += -I$(VMCWORKDIR)/r3bdata/califaData
CFLAGS += -I$(VMCWORKDIR)/r3bdata/mwpcData
CFLAGS += -I$(VMCWORKDIR)/r3bdata/neulandData
CFLAGS += -I$(VMCWORKDIR)/r3bdata/footData
CFLAGS += -I$(VMCWORKDIR)/r3bsource/base
CFLAGS += -I$(VMCWORKDIR)/r3bgen
CFLAGS += -I$(VMCWORKDIR)/califa/pars
CFLAGS += -I$(VMCWORKDIR)/r3bbase
CFLAGS += -I$(UCESB_DIR)/hbook
CFLAGS += --std=c++17 -g -O0 -fexceptions -Wno-deprecated -lstdc++fs

LDFLAGS := $(shell $(ROOTCONFIG) --ldflags)
LDFLAGS += $(shell $(ROOTCONFIG) --libs)
LDFLAGS += -lEG $(shell $(ROOTCONFIG) --glibs)
LDFLAGS += -L$(ROOT_LIBRARY_DIR) -L$(FAIRROOTPATH)/lib
LDFLAGS += -g
LDFLAGS += -L$(VMCWORKDIR)/../build/lib -lR3BData -lR3BBase -lField -lR3BTracking -lR3BCalifa -lR3BGen
LDFLAGS += -L$(FAIRROOTPATH)/lib -lBase -lParBase -lFairTools

INCLUDEDIR=include
DIR_INC=-I$(INCLUDEDIR)
HEADERS=libs.hh
DEPS = $(patsubst %,$(INCLUDEDIR)/%,$(HEADERS))

# Get all .cpp files in the directory except for the specific file to exclude (e.g., stand_in.cpp)
SRCS := $(filter-out stand_in.cpp, $(wildcard *.cpp))

# Generate individual executables for each source file
EXECUTABLES := $(patsubst %.cpp,%,$(SRCS))

# Rule to compile each .cpp file into its corresponding object file
%.o: %.cpp $(DEPS)
	@$(CXX) $(CFLAGS) $(DIR_INC) -c $< -o $@

# Rule to link each object file to its corresponding executable
$(EXECUTABLES): % : %.cpp
	@$(CXX) $(LDFLAGS) $(CFLAGS) -o $@ $< $(DIR_INC)
	@echo " COMP $@"

# Default target to build all executables
all: $(EXECUTABLES)

clean:
	rm -f *.o $(EXECUTABLES)
	rm -rf *.dSYM
