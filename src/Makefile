all: cohen_sutherland.o
	gcc cohen_sutherland.o main.c -o clip -lGL -lGLU -lglut -lm

cohen_sutherland.o: cohen_sutherland.h
	gcc -c cohen_sutherland.c

clean:
	rm -rf *.o
	rm -rf clip

run:
	./clip ../instances/test.txt 720 1280
