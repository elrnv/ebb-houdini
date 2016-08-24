# Makefile
# 
# Usage: Place this in the same directory as SOP_Star.C, and then from that
# directory run: make install
#
#CXX = clang-omp

EBB := ebb
TERRA := $(EBB)/terra
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	DSONAME = libEbb.so
	INCDIRS = -fopenmp -I/home/elarionov/proj/eigen -I./solver/ -I$(TERRA)/include -I$(EBB)/include
	LIBDIRS = -L $(TERRA)/lib
	LIBS = -lterra
else ifeq ($(UNAME_S),Darwin)
	DSONAME = libEbb.dylib
	INCDIRS = -I/usr/local/include/eigen3 -I./solver/
endif

SOP_SOURCES = SOP/SOP_Ebb.C
SOURCES = main.C $(SOP_SOURCES)
#OPTIMIZER = -g
OPTIMIZER = -O3

###############################################################################
# For GNU make, use this line:
#      include $(HFS)/toolkit/makefiles/Makefile.gnu
#
include $(HFS)/toolkit/makefiles/Makefile.gnu

