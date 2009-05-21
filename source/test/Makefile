all:
	cp ../global.h .
	cp ../Grid.* .
	cp ../Patch.* .
	cp ../Ant.* .
	g++ -c *.cpp
	rm test.o
	g++ *.o test.cpp -o test

clean:
	rm global.h Grid.* Patch.* Ant.* test
