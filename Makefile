mandelbrot: main.cpp
	mkdir images
	g++ main.cpp  -Wall -lSDL2main -lSDL2 -o mandelbrot
	./mandelbrot

clear:
	rm mandelbrot
	rm -rf images