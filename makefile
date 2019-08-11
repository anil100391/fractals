all : mandelbrot tricorn poster

mandelbrot:
	g++ -o mandelbrot mandelbrot.cpp -O3 -lcairo

tricorn:
	g++ -o tricorn tricorn.cpp -O3 -lcairo

poster:
	g++ -o orbit mandelbrotOrbit.cxx -lcairo -O3

clean:
	rm mandelbrot
	rm tricorn
	rm orbit
