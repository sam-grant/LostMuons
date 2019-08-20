C++ = g++
C_FLAGS = -g

ROOTCFLAGS=$(shell root-config --cflags)
ROOTLIBS=$(shell root-config --libs)

LOCALCFLAGS=-I/scratch/gm2/gm2-git/Fitting/install/include
LOCALCLIBS=-L/scratch/gm2/gm2-git/Fitting/install/lib -lBlinders -lRootManager -lRandom -L/usr/lib/x86_64-linux-gnu -lssl -lcrypto 


all : EuropaFitter_module

clean:
	rm EuropaFitter_module

EuropaFitter_module: EuropaFitter_module.cc EuropaFitterFunctions.hh
	$(C++) $(ROOTCFLAGS) $(LOCALCFLAGS) $(C_FLAGS)  $<  -o $@ $(ROOTLIBS) $(LOCALCLIBS)




