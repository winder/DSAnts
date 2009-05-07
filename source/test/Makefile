all:
	cp ../global.h .
	cp ../Underground.* .
	cp ../DirtDisk.* .
	cp ../Patch.* .
	cp ../Dirt.* .
	cp ../Empty.* .
	cp ../Barrier.* .
	cp ../Ant.* .
	g++ -c *.cpp
	rm test.o
	g++ *.o test.cpp -o test

clean:
	rm global.h Underground.* DirtDisk.* Patch.* Dirt.* Empty.* Barrier.* Ant.* test
