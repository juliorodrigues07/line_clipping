all: cohen_sutherland.o
	g++ cohen_sutherland.o main.cpp -o clip -lGL -lGLU -lglut -lm

cohen_sutherland.o: cohen_sutherland.hpp
	g++ -c cohen_sutherland.cpp

clean:
	rm -rf *.o
	rm -rf clip

run:
	./clip ../instances/test.txt 720 1280
