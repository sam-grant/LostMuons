C++ = g++
C_FLAGS = -g

RANDOMCFLAGS=-I/scratch/gm2/gm2-git/Fitting/install/include
ROOTCFLAGS=$(shell root-config --cflags)
ROOTLIBS=$(shell root-config --libs)



all : libBlinders.so libRootManager.so

clean:
	rm *.so

libBlinders.so: Blinders.cc Blinders.hh
	$(C++) -c $(RANDOMCFLAGS) Blinders.cc -shared -o libBlinders.so
	cp Blinders.hh /scratch/gm2/gm2-git/Fitting/install/include/blinders/
	cp libBlinders.so /scratch/gm2/gm2-git/Fitting/install/lib/

libRootManager.so: RootManager.cc RootManager.hh
	$(C++) -c $(ROOTCFLAGS) RootManager.cc -shared -o libRootManager.so
	cp RootManager.hh /scratch/gm2/gm2-git/Fitting/install/include/RootManager/
	cp libRootManager.so /scratch/gm2/gm2-git/Fitting/install/lib/

